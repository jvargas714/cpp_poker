//
// Created by Jay on 7/1/2015.
//

#ifndef GLOBAL_POKER_API_POKER_ERROR_H
#define GLOBAL_POKER_API_POKER_ERROR_H
#include <stdexcept>
#include <string>
namespace poker_error {

    using std::runtime_error;

    class HandIDError : public runtime_error {
    public:
        HandIDError();
        explicit HandIDError(const std::string &msg);
        ~HandIDError()override=default;
    };

    class FindWinnerError : public runtime_error {
    public:
        FindWinnerError();
        explicit FindWinnerError(const std::string &msg);
        ~FindWinnerError()override=default;
    };

    class FindPlayerError : public runtime_error {
    public:
        FindPlayerError();
        explicit FindPlayerError(const std::string &msg);
        ~FindPlayerError()override=default;
    };

    class CfgLoadError : public runtime_error {
    public:
        CfgLoadError();
        explicit CfgLoadError(const std::string& msg);
        ~CfgLoadError()override=default;
    };

    class InvalidHandError : public runtime_error {
    public:
        InvalidHandError();
        explicit InvalidHandError(const std::string& msg);
        ~InvalidHandError()override=default;
    };
} // END poker error namespace
#endif //GLOBAL_POKER_API_POKER_ERROR_H
