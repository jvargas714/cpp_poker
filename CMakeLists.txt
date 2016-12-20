cmake_minimum_required(VERSION 3.6)
project(poker)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DTESTING)
set(LIB_FILES
        card.cpp
        deck.cpp
        player.cpp
        poker_types.h
        poker.cpp
        TexasHoldem.cpp
        jsoncpp/jsoncpp.cpp
        poker_fsm.cpp
        brain.cpp
        bot_player.cpp
        assessment.cpp
        poker_error.cpp
        logger.cpp
        )

add_library(poker STATIC ${LIB_FILES})
add_library(pokerTest test_suite.cpp test_suite.h)
target_link_libraries(pokerTest poker)
add_executable(test_suite testing.cpp)
target_link_libraries(test_suite pokerTest)
