#include "test_suite.h"

void display_help();

TEST_CASE handle_args( int cnt, char** args );

int main( int argc, char* argv[] )
{
	poker_tester tester;
	tester.run_tests(
			handle_args( argc, argv )
	);
	return 0;
}

void display_help()
{
	using std::cout;
	using std::endl;
	cout
			<< "\n\n#############################Poker Testing Suite!!!!!##############################\n\n";
	cout << "Options:\n\t";
	cout << "( -h, --help )\t\tHelp Menu\n\t";
	cout << "( --chen [x] )\t\tCalculate preflop hand strength x times\n\t";
	cout << "( --hands [x] )\t\tDeal 5 cards and have analyzer determine hand type and hand"
			" strength [x] times\n\t";
	cout << "( --compare [x] [y])\t[y] players are dealth [x] hands and their hands are compared,"
			"a winner is decided.\n\t";
	cout << "( --simgame [x] [y] )\tSimulate game for [x] hands with [y] players "
			"(from 2 to 8 players). Will be computer against computer "
			"simulated game. Bots only." << endl;
	cout << "( --all [x] [y] )\tRun all test cases for [x] hands and [y] number of players."
		 << endl;
	cout
			<< "######################################################################################";
	cout << endl;
}

TEST_CASE handle_args( int cnt, char** args )
{
	std::map<std::string, OPTS>::const_iterator it;
	TEST_CASE test;
	std::cout << "Test type: " << args[1] << std::endl;
	it = ARG_MAP.find( args[ 1 ] );
	if ( it == ARG_MAP.end() )
	{
		std::cerr << "Error:: Invalid input, see help menu." << std::endl;
		exit( -1 );
	}
	switch ( it->second )
	{
		case HELP:
			display_help();
			break;

		case HANDS:
			if ( cnt < 2 )
			{
				std::cerr << "Error:: for hands mode you must enter number of hands that are "
						"to be simulated." << std::endl;
				exit( -1 );
			}
			test.type 		= HANDS;
			test.n_tests 	= atoi( args[ 2 ] );
			break;

		case CHEN:
			if ( cnt < 2 )
			{
				std::cerr << "Error:: for CHEN mode you must enter number of hands that are "
						"to be simulated." << std::endl;
				exit( -1 );
			}
			test.type 		= CHEN;
			test.n_tests 	= atoi( args[ 2 ] );
			break;

		case COMPARE:
			if ( cnt < 3 )
			{
				std::cerr << "Error:: for COMPARE mode you must enter number of hands that are "
						"to be simulated and number of players, see help menu." << std::endl;
				exit( -1 );
			}
			test.type 		= COMPARE;
			test.n_tests 	= atoi( args[ 2 ] );
			test.n_players 	= atoi( args[ 3 ] );
			break;

		case SIMGAME:
			if ( cnt < 3 )
			{
				std::cerr << "Error:: for SIMGAME number of hands and number of players must "
						"be entered, see help menu." << std::endl;
				exit( -1 );
			}
			test.type 		= SIMGAME;
			test.n_tests 	= atoi( args[ 2 ] );
			test.n_players 	= atoi( args[ 3 ] );
			break;

		case CONTINUOUS:
			test.type = CONTINUOUS; // test ends with cntrl + c
			break;

		case ALL:
			if ( cnt < 2 )
			{
				std::cerr << "Error" << std::endl;
				exit( -1 );
			}
			test.type = ALL;
			break;
		default:
			std::cerr << "Error:: invalid input, run with --help or -h for assistance."
					  << std::endl;

	}
	return test;
}
