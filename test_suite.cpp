#include "master_poker.h"
#include "test_suite.h"

// global logger reference
logger& lg = logger::instance( );

void test_bot_chen_hand( int num_tests )
{
	lg << "--------------------------------Testing bot chen Formulation-----------------------------\n";
	bot_player bot;
	for ( int i = 0; i < num_tests; i++ )
	{
		Card cd;
		Card cd2;
		lg << cd << "\n";
		lg << cd2 << "\n";
		bot.hand.push_back( cd );
		bot.hand.push_back( cd2 );
		bot.computeStartingPower();
		lg << bot.getBotStatus();
		bot.setStartingPower( 0 );
		bot.hand.clear();
		lg << "___________________________________________________________________________________\n";
	}
	lg << "--------------------------------Testing bot chen Formulation-----------------------------\n";
}

void test_bot_ctors()
{
	lg << "--------------------------------Testing bot ctors--------------------------------------\n";


	bot_player bot;
	bot_player bot2( "McDousherstein", 2500 );

	lg << "-----bot-------" << std::endl;
	lg << bot << std::endl;

	lg << "------bot2-----\n";
	lg << bot2 << "\n";
	lg << "----------------------------------END bot ctors------------------------------------------\n" << std::endl;
}

// run through a number of tests and log results check for accuracy
void test_hand_detection( const int& num_tests, bool random )
{
	Cards test_hand;
	lg << "---------------------Beginning Test Hand Detection-----------------\n";
	if ( random )
	{
		for ( int i = 0; i < num_tests; ++i )
		{
			test_hand = generate_hand();
			assessment::findHandStrength( test_hand );
		}
	}
	else
	{
		// TODO :: build one of each hand type and test it
		;
	}
	lg << "---------------------END Test Hand Detection-----------------\n";
}

// helpers
Cards generate_hand()
{
	Cards hand;
	for( int i = 0; i < 5; ++i )
	{
		hand.push_back( Card() );
	}
	return hand;
}





