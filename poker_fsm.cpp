#include "poker_fsm.h"

int pre_flop_bets( TexasHoldem& game )
{
	/*
	 * After cards are dealth the person infront of the dealer will have the small blind
	 * 1. small blind bet
	 * 2. big blind bet
	 * 3. the rest of players raise big blind, call, fold, or check (if can)
	 *
	 * TODO: put case for when there are two players left in the game
	 */

	for ( auto& plyr : game.players )
	{
		if ( plyr.dealer )
		{
			// TODO: need to design bot players first (use polymorph, where real players will wait
			// on a socket for a command on what to do, where a bot will automatically place a bet
			// using some ghetto AI, we want override the betting methods basically
			;
		}
	}

	return 0;

}
