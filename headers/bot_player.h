/*
 * bot_player.h
 *
 *  Created on: Jun 27, 2016
 *      Author: jay
 */

#ifndef HEADERS_BOT_PLAYER_H_
#define HEADERS_BOT_PLAYER_H_
#include "master_poker.h"
#include "player.h"
#include <map>
#include <cstdlib>
#include <cmath>

/*
 *
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
 * 	1. use chen formula to determine startingPower
 * 	2. implement assess table function to determine viability of a bet
 * 	3. create an appropriate data structure to assess cards on table to help with betting strategy
 * 	4. implement bet function based on state of game, consider starting power and chances_of_victory
 *
 */

// static const here in the head ensures that only one copy of this map is generated per cpp file
// that includes this file
static const std::map<std::string, float> chen_map
{
		{"2",  1}, {"3", 1.5},
		{"4",  2}, {"5", 2.5},
		{"6",  3}, {"7", 3.5},
		{"8",  4}, {"9", 4.5},
		{"10", 5}, {"J",   6},
		{"Q",  7}, {"K",   8},
		{"A",  10}
};

class bot_player : public Player {

public:
	bot_player();
	bot_player(string name, int cash);
	~bot_player();
	int bet(int);
	int bet();
	void setStartingPower(int amt);

private:
	// data members
	double 	chancesOfVictory;
	int 	startingPower;

	// assesses table assigns a probability of victory, will be based on the amt of cards out
	void assessTable(const Cards& cds);

public:
	// uses Chen Formula to find starting hand power
	void computeStartingPower();
	void getBotStatus() const;
};
#endif /* HEADERS_BOT_PLAYER_H_ */
