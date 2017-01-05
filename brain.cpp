#include "brain.h"
#include "logger.h"

Brain::Brain()
{ game = nullptr; }

Brain::~Brain()
{}

// TOOD:: needs testing 
bool Brain::assessTable( Player* bot, poker_states state )
{    /* TODO:: Implement assessTable function to switch through each state of the game */
	switch ( state )
	{
		case FLOP:
			assessFlopScenario( bot );
			break;
		case DEAL_HOLE_CARDS:
			break;
		case PREFLOP:
			break;
		case ROUND_OF_BETS:
			break;
		case DEAL_FLOP:
			break;
		case DEAL_TURN:
			break;
		case TURN:
			break;
		case DEAL_RIVER:
			break;
		case RIVER:
			break;
		case SHOWDOWN:
			break;
		default:
			break;
	}
	return true;
}

bool Brain::assessTable( const Cards& cds )
{
	if ( cds.size() != 5 )
	{
		log() << "Brain::assessTable(): need 5 cards present, exiting ..." << "\n";
		return false;
	}


	return false;
}

//--------------------------------------------------JayBrain----------------------------------------
JayBrain::JayBrain() : Brain()
{
	// will need to call init() with a poker game when it becomes available 
}

JayBrain::JayBrain( TexasHoldem* pkr ) : Brain()
{
	init( pkr );
}

JayBrain::~JayBrain()
{}

void JayBrain::init( TexasHoldem* pkr )
{
	// txGame should be on heap, from outside this class, point to game at hand  
	game = pkr;
}

// TODO:: needs testing 
bool JayBrain::assessFlopScenario( Player* bot )
{
	/*
		1. analyze flop situation
		2. find out what hand we are closest to and use that hand to go for
		3. calculate pot odds
		4. calculate card odds
		5. use card and pot odds to inform decision
		6. take into consideration actions of other players in game
		7. make bet based on data collected
	*/
	size_t numCardsDealt = 52 - game->gameDeck.numCardsLeft();
	int outs;
	Card hole1 = bot->hand[ 0 ];
	Card hole2 = bot->hand[ 1 ];

	if ( game->tableCards.size() != 3 )
	{
		log() << "JayBrain::assessFlopScenario(): Error -->"
				"tableCards.size() != 3, exiting" << "\n";
		return false;
	}
	// first we see if we have a valid hand off the flop
	vector<Card>::iterator it = bot->hand.begin();
	bot->hand.insert( it, game->tableCards.begin(), game->tableCards.end() );

	if ( bot->hand.size() != 5 )
	{
		return false;
	}
	// here we can run through and determine what hand type we have based on the five cards out
	bot->setHandStrength( assessment::findHandStrength( bot->hand ) );
	return true;
}
