#include "master_poker.h"
#include "test_suite.h"

poker_tester::poker_tester()
{
	log() << "Tester initialized..\n";
}


void poker_tester::test_bot_chen_hand( int num_tests )
{
	log()
			<< "--------------------------------Testing bot chen Formulation-----------------------------\n";
	bot_player bot;
	for ( int i = 0; i < num_tests; i++ )
	{
		Card cd;
		Card cd2;
		log() << cd << "\n";
		log() << cd2 << "\n";
		bot.hand.push_back( cd );
		bot.hand.push_back( cd2 );
		bot.computeStartingPower();
		log() << bot.getBotStatus();
		bot.setStartingPower( 0 );
		bot.hand.clear();
		log()
				<< "_____________________________________________________________________________________\n";
	}
	log()
			<< "--------------------------------End Testing bot chen Formulation-----------------------------\n";
}


void poker_tester::test_bot_ctors()
{
	log()
			<< "--------------------------------Testing bot ctors---------------------------------------\n";
	bot_player bot;
	bot_player bot2( "McDousherstein", 2500 );

	log() << "-----bot-------" << std::endl;
	log() << bot << std::endl;

	log() << "------bot2-----\n";
	log() << bot2 << "\n";
	log()
			<< "----------------------------------END bot ctors------------------------------------------\n"
			<< std::endl;
}


// run through a number of tests and log results check for accuracy
void poker_tester::test_hand_detection( int num_tests, bool random )
{
	Cards test_hand;
	log()
			<< "\n\n--------------------------------Beginning Test Hand Detection----------------------------\n";
	if ( random )
	{
		for ( int i = 0; i < num_tests; ++i )
		{
			test_hand = generate_hand();
			log() << display_hand( test_hand );
			assessment::findHandStrength( test_hand );
		}
	}
	else
	{
		// TODO :: build one of each hand type and test it
		;
	}
	log() << "---------------------END Test Hand Detection-----------------\n";
}

void poker_tester::compare_hands( int num_tests, const int& n_players )
{
	std::vector<Player> plyrs;
	TexasHoldem game; // already 2 players
	int cnt = 3;
	std::cout << "Number of testS: " << num_tests << std::endl;
	// fill players
	for( int n = 0; n < n_players-2; ++n )
	{
		game.enterGame( "Player " + std::to_string( cnt ), 1500 );
		cnt++;
	}
	// simulate hands
	for( int i = 0; i < num_tests; ++i )
	{
		game.dealHands();
		game.dealFlop();
		game.dealTurn();
		game.dealRiver();
		game.gameStatus();
		game.findWinner();
		game.resetHand( i+1 );
	}
}


void poker_tester::run_tests( const TEST_CASE& tc )
{
	switch ( tc.type )
	{
		case CHEN:
			if( tc.n_tests < 1 )
			{
				std::cerr << "Error invalid number of tests, must be greater than 1" << std::endl;
			}
			test_bot_chen_hand( tc.n_tests );
			break;

		case HELP:
			std::cerr << "Help case does not apply here." << std::endl;
			break;

		case HANDS:
			if( tc.n_tests < 1 )
			{
				std::cerr << "Error invalid number of tests, must be greater than 1" << std::endl;
			}
			test_hand_detection( tc.n_tests );
			break;

		case COMPARE:
			compare_hands( tc.n_tests, tc.n_players );
			break;

		case SIMGAME:
			break;

		case CONTINUOUS:
			break;

		case ALL:
			break;
	}
}

// helpers
Cards generate_hand()
{
	Cards hand;
	for ( int i = 0; i < 5; ++i )
	{
		hand.push_back( Card() );
	}
	return hand;
}

std::string display_hand( Cards& cds )
{
	std::stringstream ss;
	ss << "Cards dealt:\n";
	for ( auto& cd : cds )
	{
		ss << "\t";
		ss << cd << "\n";
	}
	return ss.str();
}
