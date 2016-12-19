#include "master_poker.h"
using namespace std;
const char* cfg_file;

int handle_args( int arg_cnt, char* args[] )
{
	if ( arg_cnt == 0 )
	{
		cerr << "Must enter configuration path" << endl;
		return -1;
	}
	cfg_file = args[ 1 ];
}

int main( int argc, char* argv[] )
{
	cout << "Here we go ya dick" << endl;
	//handle_args(argc, argv);
	string fl = "/home/jay/poker/poker/config/TXHldm.json";
	TexasHoldem game( fl );

	//state machine
	/*
	 * 1. Assign dealer, small blind, big blind (players that need to bet) by random
	 * 2.
	 */
	return 0;
}
