#include "brain.h"

using namespace std;

Brain::Brain()
{}

Brain::~Brain()
{}

// TOOD:: needs testing 
void Brain::assessTable( const Cards& cds, poker_states state )
{	/* TODO:: Implement assessTable function to switch through each state of the game */
	switch(state)
	{
		case: 
	}
}

JayBrain::JayBrain() : Brain()
{
	// will need to call init() with a poker game when it becomes available 
}

JayBrain::JayBrain( Poker* pkr ) : Brain()
{
	init( pkr );
}

~JayBrain::JayBrain()
{}

void JayBrain::init( Poker* pkr )
{
	// txGame should be on heap, from outside this class, point to game at hand  
	game = pkr;
}

// TODO:: needs testing 
void JayBrain::assessFlopScenario( const Cards& cds )
{
	/*
		- analyze flop situation
		- take into consideration actions of other players in game
		- input: 
	*/
	int cardsDealt = game.numPlayers*2 + 3;
	int outs;

}