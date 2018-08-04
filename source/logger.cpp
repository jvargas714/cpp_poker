#include <chrono>   // std::chrono::system_clock::now() and std::time_point
#include <ctime>    // std::strftime, std::time(), std::put_time, std::localtime
#include <sstream>
#include <thread>
#include <logger.h>

// FORMAT  ::>>>  [Thu Aug  2 20:28:31 2018] <GSVR> ::TRACE:: this is an example log entry

std::thread g_QProcThread;

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

//std::string log_filename;
//std::queue<S_LOG_ITEM> log_items;
//std::ostream strm;
//LOG_LEVEL current_level;
//LOG_MODULE current_module;
//bool output_stdout;
//std::mutex mtx;
//bool log_proc_is_alive;
logger::logger() noexcept :
        log_filename(gen_def_filename()),
        strm(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(LOG_LEVEL::DEBUG),
        current_module(LOG_MODULE::GENERAL),
        output_stdout(true),
        mtx(),
        log_proc_is_alive(true) {
}

logger::logger(const std::string &filename, bool log_to_stdout) :
        log_filename(filename),
        strm(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(LOG_LEVEL::DEBUG),
        current_module(LOG_MODULE::GENERAL),
        output_stdout(log_to_stdout),
        mtx(),
        log_proc_is_alive(true) {
    std::cout << "logger ";
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

logger& logger::get_instance(const std::string& filename) {
    if (!lg)
        lg = std::make_unique<logger>(filename);
    return *lg;
}

void logger::flush_logs(bool new_line) {
    for (auto &entry: files_map)
        if (new_line) *entry.second << END;
        else entry.second->flush();
}

// write general INFO log to all open files
template<typename ToPrint>
logger& logger::operator << (const ToPrint &msg) {
    S_LOG_ITEM item;
    std::stringstream ss;
    ss << msg;
    std::string tmp(ss.str());

    for (auto& entry : files_map) {
        if (entry.second->is_open()) {
            item.lvl = current_level;
            item.filename = entry.first;
            item.msg = tmp;
            item.mdl = LOG_MODULE::GENERAL;
            mtx.lock();
            log_items.push(item);
            mtx.unlock();
        }
    }
    return *lg;
}

template<typename toLog>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, toLog& msg) {
    S_LOG_ITEM item;
    item.mdl = mdl;
    item.lvl = lvl;
    std::stringstream ss;
    ss << msg;
    item.msg = ss.str();

}

void logger::set_level(LOG_LEVEL lvl) {
    this->current_level = lvl;
}

void logger::log_processing() {
    while (log_proc_is_alive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_MILLI_SLEEP));
        if (!log_items.empty()) {
            mtx.lock();
            S_LOG_ITEM item = log_items.front();
            log_items.pop();
            mtx.unlock();
            auto entry = files_map.find(item.filename);
            std::string tmp =
                    "[" + now_is() + "] " + log_module(item.mdl) + " " + log_level(item.lvl) + item.msg;
            if (entry == files_map.end()) {
                files_map[item.filename] =
                        std::make_unique<std::ofstream>(LOG_DIR+"/"+item.filename, std::ios::app);
                if (files_map[item.filename]->is_open())
                    *files_map[item.filename] << tmp << END;
            } else {
                if (entry->second->is_open())
                    *entry->second << tmp << END;
            }
        }
    }
}

logger::logger(const std::string &filename) {

}

void Q_Proc_Thread() {
    g_QProcThread = std::thread(&logger::log_processing, logger::get_instance());
}
