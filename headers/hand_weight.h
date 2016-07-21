/*
 Created by jay on 6/7/15.
 Description:
    This file contains constants and data structures to determine the value of a
    given hand.
    -- hand values will be mapped to a HashMap container
 */

#ifndef POKER_HAND_WEIGHT_H
#define POKER_HAND_WEIGHT_H

#include <map>
#include <string>
#include <array>
#include "card.h"
#include <vector>       /*
                        [0  1  2  3  4  5  6  7  8   9  10 11 12]
                        [2  3  4  5  6  7  8  9  10  j  q  k  a]
                        */
namespace HandMapper {
    static const std::array<int, 13> high{{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}};                                  // highest card wins

    static const std::array<int, 13> pair{{15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}};                                  // highest pair wins

    static const std::array<int, 13> twoPair{{26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38}};                       // highest pair wins

    static const std::array<int, 13> trips{{39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51}};                         // rank of trips sets weight

    static const std::array<int, 13> straight{{52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64}};                      // highest ranked card in straight sets weight

    static const std::array<int, 13> flush{{65,66,67,68,69,70,71,72,73,74,75,76,77}};                                     // highest ranked card sets weight

    static const std::array<int, 13> fullHouse{{78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90}};                     // high three of a kind sets weight

    static const std::array<int, 13> fourOfAKind{{91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103}};               // size of four of kind sets weight

    static const std::array<int, 13> straightFlush{{104,105,106,107,108,109,110,111,112,113,114,115,116}};                // high card sets score [index of high card]

    static const std::array<int, 13> royalFlush{{150,150,150,150,150,150,150,150,150,150,150,150,150}};                   // all same weights


    // putting it all together
    static const std::map<std::string, std::array<int, 13> > handMap
            {
             {"high",          high},
             {"pair",          pair},
             {"twoPair",       twoPair},
             {"trips",         trips},
             {"straight",      straight},
             {"flush",         flush},
             {"fullHouse",     fullHouse},
             {"fourOfAKind",   fourOfAKind},
             {"straightFlush", straightFlush},
             {"royalFlush",    royalFlush}
            };


}   // end namespace HandMapper


#endif //POKER_HAND_WEIGHT_H
