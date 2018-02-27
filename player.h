// Created by jay on 6/6/15.
#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include "poker_types.h"
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include "logger.h"
#include "card.h"


typedef std::vector<Card> Cards;
struct Hand;

/* TODO :: this class needs a redesign, should not have public data */
class Player
{
public:
	bool smallBlind_bet;
    bool bigBlind_bet;
    bool dealer;
	int cash;
    int handStrength;
    int highCardRnk;
	std::string name;
	Cards hand;
	Cards bestHand;

	Player();
	Player( std::string name, int cash );
	virtual ~Player();
	void namePlayer( std::string name );
	virtual int bet( int amt );
	virtual int bet();
	void collectPot( int amt );
	void setBestHand( Cards& cards );
	void setHandStrength( const std::string& handType, int& rank );
	virtual void setHandStrength( int val ) { handStrength = val; }
	void clearHand();
	void clearHandStrength();
	virtual void setCurrentHandStrength( const int& amt ) { handStrength = amt; }  // TODO :: needs implentation
    virtual void setCurrentBestHand( const Hand& ) { ; }
	static bool cmpCash( const Player& plyr1, const Player& plyr2 );
	bool operator==( const Player& plyr ) const;
	bool operator<( const Player& plyr ) const;
	bool operator>( const Player& plyr ) const;
	void reset();
	friend std::ostream& operator<<( std::ostream& os, const Player& plyr );
};

struct HandComparator
{
	bool operator()( const Player& p1, const Player& p2 ) const;
};

struct HighCardRnkComparator
{
	bool operator()( const Player& p1, const Player& p2 ) const;
};

#endif //POKER_PLAYER_H
