cmake_minimum_required(VERSION 3.6)
project(poker)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_definitions( -DTESTING )
include_directories("./include .")
set(LIB_FILES
    logger.h
    logger.cpp
    card.cpp
    deck.cpp
    player.cpp
    poker.cpp
    TexasHoldem.cpp
    jsoncpp/jsoncpp.cpp
    poker_fsm.cpp
    brain.cpp
    bot_player.cpp
    assessment.cpp
    poker_error.cpp)

#set( CMAKE_EXE_LINKER_FLAGS "-v" )
add_library( poker STATIC ${LIB_FILES} )

# test suite
#add_library( pokerTest STATIC test_suite.cpp test_suite.h )

# link poker lib to test suite
#target_link_libraries( pokerTest poker )

# compile executable
# add_executable( testing test_suite.cpp test_suite.h testing.cpp )
# add_executable( testing testing.cpp )

# link lib to exe
# target_link_libraries( testing pokerTest )

# scrap
add_executable( scrap_exe scrap.cpp )
#target_link_libraries( scrap_exe poker )
