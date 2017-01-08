/*
 * bot_player.h
 *
 *  Created on: Jun 27, 2016
 *      Author: jay
 */

#ifndef HEADERS_BOT_PLAYER_H_
#define HEADERS_BOT_PLAYER_H_

#include "poker_types.h"
#include "brain.h"
#include "poker_fsm.h"
#include <map>
#include <cstdlib>
#include <cmath>

/*
 * AI player, to play against
 * Override bet function
 * arg of bet will be disregarded
 *
 * Starting hand values use Chen's formula and hands use  Sklansky and Malmuth hand groups
 * REF:: https://en.wikipedia.org/wiki/Texas_hold_%27em_starting_hands
 *
 * Starting hand betting move will be based on probability of best starting hand x/20 where 20 is AA
 * Another betting factor will be the players temperment (is he a puss or will he go big)
 *
 * TODO:
 * 	1. use chen formula to determine startingPower ** done **
 * 	2. implement assess table function to determine viability of a bet
 * 	3. create an appropriate data structure to assess cards on table to help with betting strategy
 * 	4. implement bet function based on state of game, consider starting power and chances_of_victory
 * 	5. Maybe define some personality traits the bot can exhibit in its betting habits
 */

// static const here in the head ensures that only one copy of this map is generated per cpp file
// that includes this file
static const std::map<std::string, float> chen_map
		{
				{ "2",  1 },
				{ "3",  1.5 },
				{ "4",  2 },
				{ "5",  2.5 },
				{ "6",  3 },
				{ "7",  3.5 },
				{ "8",  4 },
				{ "9",  4.5 },
				{ "10", 5 },
				{ "J",  6 },
				{ "Q",  7 },
				{ "K",  8 },
				{ "A",  10 }
		};

class Brain;

typedef std::vector<Card> Cards;

class bot_player : public Player
{
	friend class Brain;

public:
	bot_player();
	bot_player( string name, int cash );
	~bot_player();
	int bet( int );
	int bet();
	void setStartingPower( int amt );
	void computeStartingPower();    // TODO:: should be moved to brain
	std::string getBotStatus() const;
	void initBrain( TexasHoldem& game );

	inline int getCurrentHandStrength() { return currentBestHand.strength; }

	inline void setCurrentHandStrength( int amt ) { currentBestHand.strength = amt; }
	inline void setCurrentBestHand( Hand hand ) { currentBestHand = hand; }

private:
	// data members
	float 	chancesOfVictory;
	int 	startingPower;
	HAND 	currentBestHand;
	Brain* 	brain;

	// Helper functions
	bool assessTable( const Cards& cds, poker_states state );
	bool findValidHand();
};

#endif /* HEADERS_BOT_PLAYER_H_ */
