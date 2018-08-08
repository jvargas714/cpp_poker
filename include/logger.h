#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>  // ofstream
#include <iomanip>  // std::put_time
#include <iostream> // std::cerr 
#include <cstdio>
#include <cstring>  // std::strcat
#include <cstdlib>
#include <memory>   // unique_ptr
#include <queue>
#include <map>
#include <mutex>
#include <thread>
#include <sstream>
#include <ctime>

#define THREAD_MILLI_SLEEP 100
#define LOCK_GUARD std::lock_guard<std::mutex> lock(mtx)

// FORMAT  ::>>>  [Thu Aug  2 20:28:31 2018] <GSVR> ::TRACE:: this is an example log entry

std::thread g_QProcThread;
void Q_Proc_Thread();

#define fstr()  std::string(__func__) + "(): "
#define FN      fstr()
#define END     std::endl
#define COUT    std::cout
#define CERR    std::cerr

#define LOG_DIR std::string ("./log/current")
#define DEFAULT_LOG_FILENAME LOG_DIR +
#define BACKUP_LOG_DIR std::string ("./log/backups")

// output to COUT as well
#define OUTPUT_COUT 1

enum class LOG_MODULE {
    POKER_GAME,
    GAME_SERVER,
    TESTING,
    GAME_SERVER_CLIENT,
    GENERAL,
    TEXASHOLDEM
};

enum LOG_LEVEL {
    INFO,
    TRACE,
    DEBUG,
    WARNING,
    ERROR
};

struct S_LOG_ITEM {
    LOG_MODULE mdl;
    LOG_LEVEL lvl;
    std::string msg;
    S_LOG_ITEM(): mdl(LOG_MODULE::GENERAL), lvl(LOG_LEVEL::TRACE), msg() { }
    S_LOG_ITEM(LOG_MODULE mdl, LOG_LEVEL lvl, std::string&& msg) : mdl(mdl), lvl(lvl), msg(std::move(msg)) { }
};

const std::string log_module(LOG_MODULE mld);
const std::string log_level(LOG_LEVEL lvl);

const LOG_LEVEL DEFAULT_LOG_LEVEL = LOG_LEVEL::INFO;
const LOG_MODULE DEFAULT_MODULE = LOG_MODULE::GENERAL;

class logger {
public:
    static bool instCreated;

    static logger& get_instance(const std::string& filename="");

    ~logger() {
        if (logger::log_proc_is_alive) log_proc_is_alive = false;
        if (logger::instCreated) delete lg;
    };

    void flush_log(bool new_line = false);

    bool set_log_file(const std::string&);

    inline void toggle_std_out() { output_stdout = !output_stdout; }

    inline void stopLogging() { logger::log_proc_is_alive = false; }

    inline bool isFileOpen() { return (strm_uptr) ? strm_uptr->is_open():false; }

    inline void set_level(LOG_LEVEL lvl = LOG_LEVEL::TRACE) { this->current_level = lvl; }

    void log_processing();

    template<typename ToPrint>
    std::ostream& operator<<(const ToPrint &msg);

    template<typename ToLog>
    void log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog* msg);

    template<typename ToLog, typename...Args>
    void log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog data, Args...args);

    template<typename ToLog, typename...Args>
    void log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog* data, Args...args);

    template<typename ToLog>
    void log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog &msg);

private:
    logger();

    explicit logger(const std::string& filename, bool log_to_stdout=true);

    template<typename ToLog, typename...Args>
    void build_stream(std::stringstream& ss, ToLog data, Args...args);

    template<typename ToLog, typename...Args>
    void build_stream(std::stringstream& ss, ToLog* data, Args...args);

    template<typename ToLog>
    void build_stream(std::stringstream& ss, ToLog* data);

    template<typename ToLog>
    void build_stream(std::stringstream& ss, ToLog data);

    // data
    std::string log_filename;
    std::queue<S_LOG_ITEM> log_items;
    std::unique_ptr<std::ofstream> strm_uptr;
    LOG_LEVEL current_level;
    LOG_MODULE current_module;
    bool output_stdout;
    std::mutex mtx;
    static bool log_proc_is_alive;
    static logger* lg;
};



// ----------------------------------------------------Implementation---------------------------------------------------
bool logger::instCreated = false;
bool logger::log_proc_is_alive = false;
logger* logger::lg = nullptr;

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

logger::logger() :
        log_filename(gen_def_filename()),
        strm_uptr(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(DEFAULT_LOG_LEVEL),
        current_module(DEFAULT_MODULE),
        output_stdout(true),
        mtx() {
    COUT << "logger with default filname: " << log_filename << " has been created" << END;
}

logger::logger(const std::string &filename, bool log_to_stdout) :
        log_filename(filename),
        strm_uptr(std::make_unique<std::ofstream>(log_filename, std::ios::app)),
        current_level(DEFAULT_LOG_LEVEL),
        current_module(DEFAULT_MODULE),
        output_stdout(log_to_stdout),
        mtx() {
    COUT << "logger with filename: " << log_filename << " has been created" << END;
}

logger& logger::get_instance(const std::string& filename) {
    if (!logger::instCreated) {
        lg = new logger( (!filename.empty()) ?
                LOG_DIR + "/" + filename :
                LOG_DIR + "/" + gen_def_filename() );
        logger::instCreated = true;
        logger::log_proc_is_alive = true;
        Q_Proc_Thread();
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
    std::stringstream ss;
    ss << msg;
    LOCK_GUARD;
    log_items.push( S_LOG_ITEM( current_module, current_level, ss.str()) );
    return *strm_uptr;
}

void logger::log_processing() {
    std::cout << __FUNCTION__ << "(): starting log processing thread " << std::endl;
    while ( logger::log_proc_is_alive ||  !log_items.empty() ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_MILLI_SLEEP));
        if (!log_items.empty()) {
            std::cout << "found item to log in queue...." << std::endl;

            mtx.lock();
            S_LOG_ITEM item = log_items.front();
            log_items.pop();
            mtx.unlock();

            std::string tmp =
                    "[" + now_is() + "] " + log_module(item.mdl) + " " + log_level(item.lvl) + " " + item.msg;
            std::cout << "LINE TO LOG: " << tmp << std::endl;

            if (item.lvl < current_level)
                continue;
            else if (strm_uptr->is_open())
                *strm_uptr << tmp << END;

            if (output_stdout) COUT << tmp << END;
        }
    }
    std::cout << "terminating log processing thread" << std::endl;
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

// non recurse const ref version
template<typename ToLog>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog& msg) {
    std::cout << __FUNCTION__ << "ref :: logging an item!!" << std::endl;
    std::stringstream ss;
    ss << msg;
    LOCK_GUARD;
    log_items.push( S_LOG_ITEM(mdl, lvl, ss.str()) );
}

template<typename ToLog>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog* msg) {
    std::cout << __FUNCTION__ << "ptr :: logging an item!!" << std::endl;
    std::stringstream ss;
    ss << *msg;
    LOCK_GUARD;
    log_items.push( S_LOG_ITEM(mdl, lvl, ss.str()) );
}

template<typename ToLog, typename... Args>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog *data, Args...args) {
    std::stringstream ss;
    build_stream(ss, data, args...);
    LOCK_GUARD;
    log_items.push( S_LOG_ITEM(mdl, lvl, ss.str()) );
}

template<typename ToLog, typename... Args>
void logger::log(LOG_MODULE mdl, LOG_LEVEL lvl, ToLog data, Args...args) {
    std::stringstream ss;
    build_stream(ss, data, args...);
    LOCK_GUARD;
    log_items.push( S_LOG_ITEM(mdl, lvl, ss.str()) );
}

template<typename ToLog, typename... Args>
void logger::build_stream(std::stringstream& ss, ToLog data, Args...args) {
    ss << data << " ";
    build_stream(ss, args...);
}

template<typename ToLog, typename... Args>
void logger::build_stream(std::stringstream& ss, ToLog *data, Args...args) {
    ss << *data << " ";
    build_stream(ss, args...);
}

template<typename ToLog>
void logger::build_stream(std::stringstream& ss, ToLog *data) {
    ss << *data << " ";
}

template<typename ToLog>
void logger::build_stream(std::stringstream& ss, ToLog data) {
    ss << data << " ";
}

void Q_Proc_Thread() {
    g_QProcThread = std::thread( &logger::log_processing, &logger::get_instance() );
}

#define LOG logger::get_instance() << FN
#define LOG_TRACE logger::get_instance() << FN
#define LOG_DEBUG logger::get_instance() << FN
#define LOG_WARNING logger::get_instance() << FN
#define LOG_ERROR logger::get_instanace() << FN
#endif  // LOGGER_H

