/*
 * poker_fsm.h
 *  Created on: Jun 21, 2016
 *      Author: jay
 *  Description:
 *  	- file with functions to manage state of game
 */
#include "master_poker.h"

#ifndef HEADERS_POKER_FSM_H_
#define HEADERS_POKER_FSM_H_

static const int TIME_DELAY = 3; // 3 second delay before the AI makes a move


enum poker_states {
	DEAL_HOLE_CARDS,
	PREFLOP,
	ROUND_OF_BETS,
	DEAL_FLOP,
	DEAL_TURN,
	DEAL_RIVER,
	SHOWDOWN,

};

enum player_actions {
	BET = 10,
	RAISE,
	FOLD,
	ALL_IN,
	CHECK
};

// pre-flop bets, returns next game state
int pre_flop_bets(TexasHoldem& game);

// round of regular bets, returns next game state
int round_of_bets(TexasHoldem& game);

// show down where remaining players reveal cards and a winner is determined or pot is split etc
int showdown(TexasHoldem& game);

#endif /* HEADERS_POKER_FSM_H_ */
