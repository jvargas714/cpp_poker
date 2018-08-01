//
// Created by jay on 12/18/16.
//
#ifndef POKER_POKER_TYPES_H_H
#define POKER_POKER_TYPES_H_H

#include <unordered_map>
#include "card.h"
#include "player.h"

class Player;
class Card;

// const strings
const std::string TEST_SUITE_LOG = "./test_suite.log";
const std::string DEBUG_LOG = "./poker_debug.log";
\
// 7 choose 5 Five cards on the table and two hole cards, combinations of 5 cards out of 7
extern const std::vector<std::vector<int>> HAND_INDEX_COMBINATIONS;

// ----------------------------------------------typedefs----------------------------------------------------------
typedef std::vector<Card> Cards;
typedef std::pair<Card, Card> HoleCards;
typedef std::unordered_map<std::string, Player> PlayerMap;
typedef std::vector<std::vector<Card>> cardSuperVector;    // card vector embedded in card set for combo Method
typedef std::vector<std::vector<int>> intComb;

// Card Rankings and Suite Indicies
typedef enum {
    TWO,    // 0
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,  // 6
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE  // 12
} CARD_RANK;

typedef enum {
    CLUB,
    DIAMOND,
    HEART,
    SPADE
} CARD_SUIT;

// -----------------------------Event and Command Types-----------------------------------
enum class HOLDEM_STATES {
    PREFLOP,
    ROUND_OF_BETS,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

/*
 *  todo :: document commands and data
 */
enum class COMMAND_TYPE {
    BET,
    RAISE,
    FOLD,
    ALL_IN,
    CHECK,
    ADD_GAME,
    FIND_WINNER,
    DEAL_HOLE_CARDS,
    DEAL_FLOP,
    DEAL_TURN,
    DEAL_RIVER,
    LEAVE_GAME,
    ENTER_GAME,
    SAVE_GAME,
    LOAD_GAME,
    SEND_ERROR
};

enum class POKER_ERROR {
    CFG_LOAD_ERROR,
    PLAYER_LOOKUP_ERROR,
    HAND_ASSESS_ERROR
};

struct HoldemAction {
    std::string playerName;
    COMMAND_TYPE command;
    std::vector<uint8_t> data;

};

struct HoldemEvent {
    uint64_t gameId;
    HoldemAction action;
};
#endif //POKER_POKER_TYPES_H_H
