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

// some macros for log prefix
#define T       "::TRACE::"
#define D       "::DEBUG::"
#define E       "::ERROR::"
#define I       "::INFO::"
#define W       "::WARNING::"

#define DEFAULT_LOG_LVL LOG_LEVEL::
#define THREAD_MILLI_SLEEP 100

#define fstr()  std::string(__func__) + "(): "
#define FN      fstr()
#define END     std::endl
#define COUT    std::cout
#define CERR    std::cerr

#define LOG logger::get_instance() << FN
#define LOG_TRACE logger::get_instance() << FN
#define LOG_DEBUG logger::get_instance() << FN
#define LOG_WARNING logger::get_instance() << FN

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
    std::string msg;
    LOG_MODULE mdl;
    LOG_LEVEL lvl;
};

const std::string log_module(LOG_MODULE mld);
const std::string log_level(LOG_LEVEL lvl);

class logger {
private:
    std::string log_filename;
    std::queue<S_LOG_ITEM> log_items;
    std::unique_ptr<std::ofstream> strm_uptr;
    LOG_LEVEL current_level;
    LOG_MODULE current_module;
    bool output_stdout;
    std::mutex mtx;
    bool log_proc_is_alive;
    static logger* lg;

protected:
    logger() noexcept;
    explicit logger(const std::string& filename, bool log_to_stdout=true);

public:
    static bool instCreated;

    static logger& get_instance(const std::string& filename="");

    ~logger() { if (instCreated) delete lg; };

    void flush_log(bool new_line = false);

    bool set_log_file(const std::string&);

    template<typename ToPrint>
    std::ostream& operator<<(const ToPrint &msg);

    template<typename toLog>
    void log(LOG_MODULE mdl, LOG_LEVEL lvl, toLog &msg);

    inline void toggle_std_out() { output_stdout = !output_stdout; }

    inline void stopLogging() { log_proc_is_alive = false; if (strm_uptr) strm_uptr->close(); }

    inline bool isFileOpen() { return (strm_uptr) ? strm_uptr->is_open():false; }

    inline void set_level(LOG_LEVEL lvl = LOG_LEVEL::TRACE) { this->current_level = lvl; }

    // logging thread to process Logging Queue Items
    void log_processing();
};

#endif  // LOGGER_H

