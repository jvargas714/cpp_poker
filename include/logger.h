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

#define LOG     log() << I << FN
#define LOG_TRACE log() << T << FN
#define LOG_DEBUG log() << D << FN
#define LOG_WARNING log() << W << FN
#define LOG_ERROR log() << E << FN

#define LOG_DIR std::string ("./log/current")
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
    std::string filename;
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
    std::map<std::string, std::unique_ptr<std::ofstream>> files_map;
    LOG_LEVEL current_level;
    LOG_MODULE current_module;
    bool output_stdout;
    std::mutex mtx;
    bool log_proc_is_alive;

    logger();

    static std::unique_ptr<logger> lg;

public:
    static logger& get_instance();

    ~logger() = default;

    void flush_logs(bool new_line = false);

    void set_level(LOG_LEVEL lvl = DEBUG);

    template<typename ToPrint>
    logger& operator<<(const ToPrint &msg);

    template<typename toLog>
    void log(LOG_MODULE mdl, toLog &msg);

    inline void toggle_std_out() { output_stdout = !output_stdout; }

    inline void stopLogging() { log_proc_is_alive = false; }

    // logging thread to process Logging Queue Items
    void log_processing();
};

#endif  // LOGGER_H

