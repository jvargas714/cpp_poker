#include "master_poker.h"
#include <iostream>
#include <cstdarg>


//#define LOG(...) LogWrapper(__FILE__, __LINE__, __va_list__)

using namespace std;
const char* cfg_file;

void handle_args( int arg_cnt, char* args[] ){
	if( arg_cnt == 0 ){
		cerr << "Must enter configuration path" << endl;
	}

	cfg_file = args[1];
}



int main( int argc, char* argv[] ) {

	cout << "Here we go ya dick" << endl;
	//handle_args(argc, argv);
	string fl = "/home/jay/poker/poker/config/TXHldm.json";
	TexasHoldem game(fl);

	//state machine
	/*
	 * 1. Assign dealer, small blind, big blind (players that need to bet) by random
	 * 2.
	 */

	return 0;
}
