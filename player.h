// Created by jay on 6/6/15.
#ifndef POKER_PLAYER_H
#define POKER_PLAYER_H

#include "poker_types.h"
#include <string>
#include <vector>
#include <cstdint>
#include "card.h"
#include "hand_weight.h"

typedef std::vector<Card> Cards;

/* TODO :: this class needs a redesign, should not have public data */
class Player
{
public:
	bool smallBlind_bet, bigBlind_bet, dealer;
	int cash, handStrength, highCardRnk;
	std::string name;
	Cards hand;
	Cards bestHand;

	Player();
	Player( std::string name, int cash );
	virtual ~Player();
	void setHand( Cards& newHand ) {log() << "implement me!" << std::endl; };
	void namePlayer( std::string name );
	virtual int bet( int amt );
	virtual int bet();
	void collectPot( int amt );
	void setBestHand( Cards& cards );
	void setHandStrength( const std::string& handType, int& rank );
	virtual bool assessTable() { return false; /* TODO :: implement this to have interaction with the human player */ }
	virtual void setHandStrength( int val ) { handStrength = val; }
	void clearHand();
	void clearHandStrength();
	virtual void setCurrentHandStrength( int amt ) { amt++; }  // TODO :: needs implentation
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


/*
 * TODO:
 * Player class needs a socket interface for player to make move
 * For integration with web app
 */
