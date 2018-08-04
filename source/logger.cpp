#include <chrono>   // std::chrono::system_clock::now() and std::time_point
#include <ctime>    // std::strftime, std::time(), std::put_time, std::localtime
#include <sstream>
#include <thread>
#include <logger.h>

// FORMAT  ::>>>  [Thu Aug  2 20:28:31 2018] <GSVR> ::TRACE:: this is an example log entry

std::thread g_QProcThread;
bool logger::instCreated = false;

void Q_Proc_Thread() {
    g_QProcThread = std::thread(&logger::log_processing, &logger::get_instance());
}

static const std::string now_is() {
    std::time_t tm = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time( std::localtime(&tm), "%c" );
    return ss.str();
}

static const std::string gen_def_filename() {
    std::stringstream ss;
    std::time_t tm = std::time(nullptr);
    ss << std::put_time(std::localtime(&tm), "LOG_%H%M__%a_%b_%d_%Y.log");  // LOG_1640__Fri_Dec_12_2018
    return ss.str();
}

const std::string log_level(LOG_LEVEL lvl) {
    switch (lvl) {
        case LOG_LEVEL::DEBUG:    return "::DEBUG::";
        case LOG_LEVEL::INFO:     return "::INFO::";
        case LOG_LEVEL::TRACE:    return "::TRACE::";
        case LOG_LEVEL::WARNING:  return "::WARNING::";
        case LOG_LEVEL::ERROR:    return "::ERROR::";
        default:                  return "";
    }
}

const std::string log_module(LOG_MODULE mld) {
    switch (mld) {
        case LOG_MODULE::GAME_SERVER:         return "<GSVR>";
        case LOG_MODULE::GAME_SERVER_CLIENT:  return "<GSVRC";
        case LOG_MODULE::POKER_GAME:          return "<PKR>";
        case LOG_MODULE::TESTING:             return "<TST>";
        case LOG_MODULE::GENERAL:             return "<GEN>";
        case LOG_MODULE::TEXASHOLDEM:         return "<TXHDM>";
    }
}

logger::logger() noexcept :
        log_filename(gen_def_filename()),
        strm_uptr(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(LOG_LEVEL::TRACE),
        current_module(LOG_MODULE::GENERAL),
        output_stdout(true),
        mtx(),
        log_proc_is_alive(true) {
    COUT << "logger with default filname: " << log_filename << " has been created" << END;
    Q_Proc_Thread();
}

logger::logger(const std::string &filename, bool log_to_stdout) :
        log_filename(filename),
        strm_uptr(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(LOG_LEVEL::TRACE),
        current_module(LOG_MODULE::GENERAL),
        output_stdout(log_to_stdout),
        mtx(),
        log_proc_is_alive(true) {
    COUT << "logger with filename: " << log_filename << " has been created" << END;
    Q_Proc_Thread();
}

logger& logger::get_instance(const std::string& filename) {
    if (!logger::instCreated) {
        lg = new logger( (filename!="") ? filename:gen_def_filename() );
        logger::instCreated = true;
    }
    return *lg;
}

void logger::flush_log(bool new_line) {
        if (new_line) {
            *strm_uptr << "\n";
            strm_uptr->flush();
        }
        else strm_uptr->flush();
}

// logs to file using current settings of the logger
template<typename ToPrint>
std::ostream& logger::operator << (const ToPrint &msg) {
    S_LOG_ITEM item;
    std::stringstream ss;
    ss << msg;
    std::string tmp(ss.str());
    item.lvl = current_level;
    item.msg = tmp;
    item.mdl = current_module;
    mtx.lock();
    log_items.push(item);
    mtx.unlock();
    return *strm_uptr;
}

// can log any item that has a stream operator << defined
template<typename toLog>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, toLog& msg) {
    S_LOG_ITEM item;
    item.mdl = mdl;
    item.lvl = lvl;

    std::stringstream ss;
    ss << msg;
    item.msg = ss.str();

    mtx.lock();
    log_items.push(item);
    mtx.unlock();
}

void logger::log_processing() {
    while (log_proc_is_alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_MILLI_SLEEP));
        if (!log_items.empty()) {
            mtx.lock();
            S_LOG_ITEM item = log_items.front();
            log_items.pop();
            mtx.unlock();

            std::string tmp =
                    "[" + now_is() + "] " + log_module(item.mdl) + " " + log_level(item.lvl) + item.msg;

            if (item.lvl < current_level)
                break;
            else if (strm_uptr->is_open()) *strm_uptr << tmp << END;

            if (output_stdout) CERR << tmp << END;
        }
    }
}

// close current file if its open otherwise open a new or different file
bool logger::set_log_file(const std::string& filename) {
    if (strm_uptr) {
        if (strm_uptr->is_open())
            flush_log(true);
        strm_uptr->close();
        strm_uptr.reset();
    }
    strm_uptr = std::make_unique<std::ofstream>(filename, std::ios::app);
    return strm_uptr->is_open();
}
