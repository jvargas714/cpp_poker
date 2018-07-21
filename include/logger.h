#ifndef LOGGER_H
#define LOGGER_H

#include "poker_types.h"
#include <fstream>  // ofstream
#include <iomanip>  // std::put_time
#include <iostream> // std::cerr 
#include <cstdio>
#include <cstring>  // std::strcat
#include <cstdlib>
#include <ctime>    // std::strftime
#include <memory>   // unique_ptr
#include <chrono>   // std::chrono::system_clock::now() and std::time_point
/*
 * TODO:: set module in logger as well, use #define to do so
 */
// some macros for log prefix
#define D       "::DEBUG::"
#define V       "::VERBOSE"
#define E       "::ERROR::"
#define I       "::INFO::"
#define fstr()  std::string(__func__) + "(): "
#define FN      fstr()

class logger;

// helper function
std::string now();

logger &log();

void delete_logger();


class logger {
    std::ostream *strm;
    std::string log_filename;
    std::string mode;
public:
    // ctor, cpy ctor, and assignment operator
    logger() : log_filename(""), mode(D) {
        strm = &std::cout;
    }

    explicit logger(std::string name) : log_filename(std::move(name)), mode(D) {
        std::cerr << "Creating a new logger writing to file name: " << log_filename << std::endl;
        strm = new std::ofstream(log_filename);
    }

    logger(std::string name, std::ios_base::openmode op_md) : log_filename(std::move(name)), mode(D) {
        strm = new std::ofstream(log_filename, op_md);
    }

    logger(const logger &lg) {
        strm = lg.strm;
        log_filename = lg.log_filename;
        mode = lg.mode;
    }

    ~logger() {
        std::cerr << "~logger(): flushing and closing " << log_filename << std::endl;
        if (strm != &std::cout && strm != nullptr) {
            strm->flush();
            delete strm;
        }
    }

    inline void flush_log(bool new_line = false) {
        if (new_line) {
            *strm << "\n";
        }
        strm->flush();
    }

    inline void set_mode(std::string md = "DEBUG") {
        mode = std::move(md);
    }

    // implicit conversion to ostream, so () operator returns
    //operator std::ostream() { return *strm; }

    template<typename toPrint>
    std::ostream &operator<<(const toPrint &msg) {
        if (strm == &std::cout) {
//			std::cout << now() << mode << " " << msg;
            std::cout << msg;
        } else {
            auto *fstrm = dynamic_cast<std::ofstream *>( strm );
            if (fstrm->is_open() && mode == "OFF") {
                std::cerr << "Mode is set to OFF, closing log_file: " << log_filename << std::endl;
                fstrm->flush();
                fstrm->close();
            } else if (fstrm->is_open()) {
//				*fstrm << now() << mode << " " << msg;
                *fstrm << msg;
                fstrm->flush();
            } else {
                std::cerr << "Error log file: " << log_filename << " is not opened." << std::endl;
            }
        }
        return *strm;
    }

    inline void log(std::string &&msg, bool log_prefix = true) {
        if (strm == &std::cout) {
            std::cout << now() << mode << " " << msg;
        } else {
            auto *fstrm = dynamic_cast<std::ofstream *>(strm);
            if (fstrm->is_open()) {
                if (log_prefix) {
                    *fstrm << now() << mode << " " << msg;
                } else {
                    *fstrm << msg;
                }
            } else {
                return;
            }
            fstrm->flush();
        }
    }
};

#endif
