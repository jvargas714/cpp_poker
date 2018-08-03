#include <chrono>   // std::chrono::system_clock::now() and std::time_point
#include <ctime>    // std::strftime, std::time(), std::put_time, std::localtime
#include <sstream>
#include <thread>
#include <logger.h>

#include "logger.h"

std::thread g_QProcThread;

static const std::string now_is() {
    std::time_t tm = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time( std::localtime(&tm), "%c" );
    return ss.str();
}

logger::logger()  :
        current_level(LOG_LEVEL::DEBUG),
        current_module(LOG_MODULE::GENERAL),
        output_stdout(true),
        mtx(),
        log_proc_is_alive(true) {}

const std::string log_level(LOG_LEVEL lvl) {
    switch (lvl) {
        case LOG_LEVEL::DEBUG:
            return "::DEBUG::";
        case LOG_LEVEL::INFO:
            return "::INFO::";
        case LOG_LEVEL::TRACE:
            return "::TRACE::";
        case LOG_LEVEL::WARNING:
            return "::WARNING::";
        case LOG_LEVEL::ERROR:
            return "::ERROR::";
        default:
            return "";
    }
}

const std::string log_module(LOG_MODULE mld) {
    switch (mld) {
        case LOG_MODULE::GAME_SERVER:
            return "<GSVR>";
        case LOG_MODULE::GAME_SERVER_CLIENT:
            return "<GSVRC";
        case LOG_MODULE::POKER_GAME:
            return "<PKR>";
        case LOG_MODULE::TESTING:
            return "<TST>";
        case LOG_MODULE::GENERAL:
            return "<GEN>";
        case LOG_MODULE::TEXASHOLDEM:
            return "<TXHDM>";
    }
}

logger& logger::get_instance() {
    if (!lg)
        lg = std::make_unique<logger>();
    return *lg;
}

void logger::flush_logs(bool new_line) {
    for (auto &entry: files_map) {
        if (new_line) *entry.second << END;
        else entry.second->flush();
    }
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
    return *this;
}

template<typename toLog>
void logger::log(LOG_MODULE mdl, toLog& msg) {
    // todo :: implement me
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

void Q_Proc_Thread() {
    g_QProcThread = std::thread(&logger::log_processing, logger::get_instance());
}