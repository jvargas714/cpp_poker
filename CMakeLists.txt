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

# build dependencies
add_subdirectory(./external/jsoncpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
include_directories(./include . ./external/jsoncpp )
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

add_library( poker STATIC ${LIB_FILES} )

# link built dependencies to poker lib
link_directories(./external/lib)
link_libraries(poker jsoncpp)

# test suite
if (TESTING_SUITE)
    include_directories( ./testing ./testing/googletest/googletest/include )
    # build google test api targets: gtest and gtest_main
    add_subdirectory( testing/googletest/googletest )

    # google test suite
    add_library( pokerGTestSuite STATIC testing/test_suite.cpp )
    target_link_libraries( pokerGTestSuite poker gtest )
    add_executable( gPokerTestingMain testing/testingMain.cpp )
    target_link_libraries( gPokerTestingMain pokerGTestSuite )

endif()

# scrap exe
add_executable(scrap_exe source/scrap.cpp)
