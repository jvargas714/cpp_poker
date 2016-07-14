/*
 * bot_player.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: jay
 */
#include "bot_player.h"

bot_player::bot_player()
: Player(), chancesOfVictory(0), startingPower(0){ name = "Bot_Player";}

bot_player::bot_player(std::string name, int cash)
: Player(name, cash), chancesOfVictory(0), startingPower(0){}

bot_player::~bot_player(){}

void bot_player::computeStartingPower()
{
	/* 	uses chen's formula to compute starting hand power
	 	input should be two hole cards of the player
	 	Bot Strategy (can use two methods):
	 		1a. always raise or reraise with 12 points or more
	 		2a. only consider calling a raise with 10 pts or more

	 		1b. Can use a ratio x/20 and convert that to a probabiltity the player should
	 			raise or not.
	 */
	std::cout << "bot_player::computeStartingPower(): " << std::endl;
	const Card& cd1	= hand[0];
	const Card& cd2 = hand[1];
	float cdPwr1 	= chen_map.at( cd1.getRank() );
	float cdPwr2 	= chen_map.at( cd2.getRank() );
	float tmp_scr  	= 0.0;
	bool  pair 		= false;

	// check for pocket pair first
	if ( cdPwr1 == cdPwr2 )
	{
		( cdPwr1*2 < 5 ) ? tmp_scr=5 : tmp_scr=cdPwr1*2;
		pair = true;
	}
	else
	{
		( cdPwr1 > cdPwr2 ) ? tmp_scr=cdPwr1 : tmp_scr=cdPwr2;
	}

	// suited?
	if(cd1.getSuit() == cd2.getSuit() )
		tmp_scr+=2;

	if( !pair )
	{
		// Calc Closeness of Both Cards
		int card_gap = abs( (cd1.rankIndex-cd2.rankIndex) )-1;
		switch( card_gap )
		{
			case 0:
				if( (cdPwr1 != cdPwr2) && ((cdPwr1 < 7) && (cdPwr2 < 7)) )
					tmp_scr+=1;
				break;
			case 1:
				if( (cdPwr1 != cdPwr2) && ((cdPwr1 < 7) && (cdPwr2 < 7)) )
					tmp_scr+=1;
				tmp_scr-=1;
				break;
			case 2:
				tmp_scr-=2;
				break;
			case 3:
				tmp_scr-=4;
				break;
			default:
				tmp_scr-=5;
				break;
		}
	}

	// round up
	startingPower = (int)round(tmp_scr);
}

void bot_player::getBotStatus() const
{
	// used for testing
	// output to standard output
	std::cout << *this;
	std::cout << "startingPower: " << startingPower << "\n";
	std::cout << "chancesOfVictory: " << chancesOfVictory << std::endl;
}

void bot_player::setStartingPower(int amt)
{
	startingPower = amt;
}

int bot_player::bet(int amt)
{
	//TODO: implement
	return 0;
}

int bot_player::bet()
{
	//TODO: implement
	return 0;
}

void bot_player::assessTable( const Cards& cds, poker_states state ) 
{
	/*
		FLOP Situation (RATIO METHOD):
			TODO: define flop situation betting
	*/
}