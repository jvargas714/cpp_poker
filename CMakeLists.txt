cmake_minimum_required(VERSION 3.6)
project(poker)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
add_definitions( -DTESTING )
set(LIB_FILES
    logger.h
    logger.cpp
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
    )

#set( CMAKE_EXE_LINKER_FLAGS "-v" )
add_library( poker STATIC ${LIB_FILES} )
add_library( pokerTest STATIC test_suite.cpp test_suite.h )
target_link_libraries( pokerTest poker )
add_executable( testing test_suite.cpp test_suite.h testing.cpp )
target_link_libraries( testing poker )

# scrap
add_executable( scrap_exe scrap.cpp )
target_link_libraries( scrap_exe poker )
