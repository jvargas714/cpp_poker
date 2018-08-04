# All Targets
# poker: poker static lib
# jsoncpp: json library for reading and writing json files (external)
# gtest: google test suite library
# pokerGTestSuite: library of testing functions and parameters
# gPokerTestingMain: testing executable application
# scrap_exe: throw away code for testing random stuff

cmake_minimum_required(VERSION 3.6)
project(poker)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(TESTING_SUITE ON)

# compile definitions
add_compile_definitions(SERVER_DEBUG)

# build dependencies
add_subdirectory(./external/jsoncpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(./include . ./external/jsoncpp )
set(LIB_FILES
        include/logger.h
        source/logger.cpp
        source/card.cpp
        source/deck.cpp
        source/player.cpp
        source/poker.cpp
        source/TexasHoldem.cpp
        source/brain.cpp
        source/bot_player.cpp
        source/assessment.cpp
        source/poker_error.cpp
        include/assessment.h
        include/bot_player.h
        include/brain.h
        include/card.h
        include/cyclic_buff.h
        include/deck.h
        include/hand_weight.h
        include/player.h
        include/poker.h
        include/poker_error.h
        include/poker_fsm.h
        include/poker_types.h
        include/TexasHoldem.h include/pokerServer.h)

add_library( poker STATIC ${LIB_FILES})

# link built dependencies to poker lib
link_directories(./external/lib)
link_libraries(poker jsoncpp)

# test suite
if (TESTING_SUITE)
    include_directories( ./testing ./testing/googletest/googletest/include )
    add_subdirectory( testing/googletest/googletest )
    add_executable( gPokerTestingMain testing/testingMain.cpp)
    target_link_libraries( gPokerTestingMain poker gtest)
endif()

# misc executables
add_executable(scrapExe source/scrap.cpp)
add_executable(pokerServer source/pokerServer.cpp)
target_link_libraries(pokerServer poker)

