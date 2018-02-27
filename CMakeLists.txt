cmake_minimum_required(VERSION 3.6)
project(poker)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# build dependencies
add_subdirectory(./external/jsoncpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_definitions( -DTESTING )
include_directories(include . ./external/jsoncpp)
set(LIB_FILES
        include/logger.h
        source/logger.cpp
        source/card.cpp
        source/deck.cpp
        source/player.cpp
        source/poker.cpp
        source/TexasHoldem.cpp
        source/poker_fsm.cpp
        source/brain.cpp
        source/bot_player.cpp
        source/assessment.cpp
        source/poker_error.cpp)

#set( CMAKE_EXE_LINKER_FLAGS "-v" )
add_library( poker STATIC ${LIB_FILES} )

# link built dependencies to poker lib
link_directories(./external/lib)
link_libraries(poker jsoncpp)

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
add_executable(scrap_exe source/scrap.cpp)
