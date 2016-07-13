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
        /* inherited functions from exception and runtime_error
            1. what() --> gets string identifying exception
         */

        public:
            HandIDError();
            HandIDError(const std::string& msg);
    };

    class FindWinnerError : public runtime_error {

    	public:
    		FindWinnerError();
    		FindWinnerError(const std::string& msg);
    };

    class FindPlayerError : public runtime_error {

    public:
    	FindPlayerError();
    	FindPlayerError(const std::string& msg);
    };
} // END poker error namespace


#endif //GLOBAL_POKER_API_POKER_ERROR_H
