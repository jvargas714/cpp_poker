//
// Created by jay on 12/18/16.
//
#ifndef POKER_POKER_TYPES_H_H
#define POKER_POKER_TYPES_H_H

#include <unordered_map>
#include "card.h"
#include "player.h"

typedef enum {
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
} CARD_RANK;

typedef enum {
    CLUB,
    DIAMOND,
    HEART,
    SPADE
} CARD_SUIT;

// 7 choose 5 Five cards on the table and two hole cards, combinations of 5 cards out of 7
const std::vector<std::vector<int>> HAND_INDEX_COMBINATIONS = {
        {0, 1, 2, 3, 4},
        {0, 1, 2, 3, 5},
        {0, 1, 2, 3, 6},
        {0, 1, 2, 5, 4},
        {0, 1, 2, 6, 4}, 
        {0, 1, 5, 3, 4},
        {0, 1, 6, 3, 4},
        {0, 5, 2, 3, 4},
        {0, 6, 2, 3, 4},
        {5, 1, 2, 3, 4},
        {6, 1, 2, 3, 4},
        {5, 6, 2, 3, 4},
        {5, 1, 6, 3, 4},
        {5, 1, 2, 6, 4},
        {5, 1, 2, 3, 6},
        {0, 5, 6, 3, 4},
        {0, 5, 2, 6, 4},
        {0, 5, 2, 3, 6},
        {0, 1, 5, 6, 4},
        {0, 1, 5, 3, 6},
        {0, 1, 2, 5, 6}
};

// const strings
const std::string TEST_SUITE_LOG = "./test_suite.log";
const std::string DEBUG_LOG = "./poker_debug.log";

class Player;
class Card;

// types
typedef std::vector<Card> Cards;
typedef std::pair<Card, Card> HoleCards;
typedef std::unordered_map<std::string, Player> PlayerMap;
typedef std::vector<std::vector<Card>> cardSuperVector;    // card vector embedded in card set for combo Method
typedef std::vector<std::vector<int>> intComb;
#endif //POKER_POKER_TYPES_H_H
