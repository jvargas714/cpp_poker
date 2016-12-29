#include "master_poker.h"
#include "test_suite.h"

poker_tester::poker_tester() :_lg( logger() ), _num_tests( TEST_CNT )
{}


poker_tester::poker_tester( std::string output_file ) : _lg( logger( output_file ) )
{}


void poker_tester::test_bot_chen_hand( int num_tests )
{
	_lg << "--------------------------------Testing bot chen Formulation-----------------------------\n";
	bot_player bot;
	for ( int i = 0; i < num_tests; i++ )
	{
		Card cd;
		Card cd2;
		_lg << cd << "\n";
		_lg << cd2 << "\n";
		bot.hand.push_back( cd );
		bot.hand.push_back( cd2 );
		bot.computeStartingPower();
		_lg << bot.getBotStatus();
		bot.setStartingPower( 0 );
		bot.hand.clear();
		_lg << "_____________________________________________________________________________________\n";
	}
	_lg << "--------------------------------Testing bot chen Formulation-----------------------------\n";
}


void poker_tester::test_bot_ctors()
{
	_lg << "--------------------------------Testing bot ctors---------------------------------------\n";
	bot_player bot;
	bot_player bot2( "McDousherstein", 2500 );

	_lg << "-----bot-------" << std::endl;
	_lg << bot << std::endl;

	_lg << "------bot2-----\n";
	_lg << bot2 << "\n";
	_lg << "----------------------------------END bot ctors------------------------------------------\n" << std::endl;
}


// run through a number of tests and log results check for accuracy
void poker_tester::test_hand_detection( int num_tests, bool random )
{
	Cards test_hand;
	_lg << "--------------------------------Beginning Test Hand Detection----------------------------\n";
	if ( random )
	{
		for ( int i = 0; i < num_tests; ++i )
		{
			test_hand = generate_hand();
			assessment::findHandStrength( test_hand );
			_lg << "\n";
		}
	}
	else
	{
		// TODO :: build one of each hand type and test it
		;
	}
	_lg << "---------------------END Test Hand Detection-----------------\n";
}


// helpers
Cards poker_tester::generate_hand()
{
	Cards hand;
	for( int i = 0; i < 5; ++i )
		hand.push_back( Card() );
	return hand;
}
