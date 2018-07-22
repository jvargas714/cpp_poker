//
// Created by jay on 12/18/16.
//
#ifndef POKER_POKER_TYPES_H_H
#define POKER_POKER_TYPES_H_H

#include <unordered_map>
#include "card.h"
#include "player.h"

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
