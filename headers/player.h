// Created by jay on 6/6/15.
#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H
#include <string>
#include <vector>
#include "card.h"
#include <cstdint>

using std::vector;
using std::string;

class Player {

public:
	bool smallBlind_bet, bigBlind_bet, dealer;
	int cash, handStrength, highCardRnk;
    string name;
    vector<Card> hand;
    vector<Card> bestHand;

    Player();
    Player(string name, int cash);
    virtual ~Player();
    void setHand(vector<Card> &newHand);
    void namePlayer(string name);
    virtual int bet(int amt);
    virtual int bet();
    void collectPot(int amt);
    void setBestHand(vector<Card> &cards);
    void setHandStrength(const string &handType, int &rank);
    void clearHand();
    void clearHandStrength();
    static bool cmpCash(const Player &plyr1, const Player &plyr2); // compares to another player (cash balance)
    bool operator == (const Player& plyr) const;
    bool operator < (const Player& plyr) const;
    bool operator > (const Player& plyr) const;
    void reset();
    friend std::ostream& operator << (std::ostream& os, const Player& plyr);
};

struct HandComparator{

    bool operator()(const Player& p1, const Player& p2)const;
};

struct HighCardRnkComparator{

    bool operator()(const Player& p1, const Player& p2) const;
};

#endif //POKER_PLAYER_H


/*
 * TODO:
 * Player class needs a socket interface for player to make move
 * For integration with web app
 */
