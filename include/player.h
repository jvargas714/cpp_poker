// Created by jay on 6/6/15.
#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include "logger.h"
#include "poker_types.h"
#include "card.h"
#include "assessment.h"


/*
 * HAND_TYPE type;
    Cards cards;
    int strength;
    std::string hand_str;
 */

typedef std::pair<Card, Card> HoleCards;

class Player {
public:
    bool smallBlind_bet;
    bool bigBlind_bet;
    bool dealer;
    int cash;
    int highCardRnk;
    std::string name;
    Hand hand;
    HoleCards holeCards;

    Player();
    Player(std::string name, int cash);
    virtual ~Player();
    void namePlayer(std::string name);
    virtual int bet(int amt);
    virtual int bet();
    void collectPot(int amt);
    void setHandStrength(const std::string &handType, int &rank);
    virtual void setHandStrength(int val) { hand.strength = val; }
    void clearHand();
    void clearHandStrength();
    bool isValidHand() const;
    virtual void setCurrentHandStrength(const int &amt) { hand.strength = amt; }  // TODO :: needs implentation
    virtual void setCurrentBestHand(const Hand &) { ; }
    static bool cmpCash(const Player &plyr1, const Player &plyr2);
    bool operator==(const Player &plyr) const;
    bool operator<(const Player &plyr) const;
    bool operator>(const Player &plyr) const;
    void reset();
    friend std::ostream &operator<<(std::ostream &os, const Player &plyr);
    void clearHoleCards();
};

struct HandComparator {
    bool operator()(const Player &p1, const Player &p2) const;
};

struct HighCardRnkComparator {
    bool operator()(const Player &p1, const Player &p2) const;
};

#endif //POKER_PLAYER_H
