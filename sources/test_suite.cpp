/*
 * test_suite.cpp
 *
 *  Created on: Jul 11, 2016
 *      Author: jay
 *
 *  General Testing file
 */
#include "master_poker.h"

using namespace std;

void test_bot_chen_hand(int num_tests)
{
	bot_player bot;
	for(int i = 0; i < num_tests; i++)
	{
		Card cd;
		Card cd2;
		cout << cd << endl;
		cout << cd2 << endl;
		bot.hand.push_back(cd);
		bot.hand.push_back(cd2);
		bot.computeStartingPower();
		bot.getBotStatus();
		bot.setStartingPower(0);
		bot.hand.clear();
		cout << "___________________________________________________________________________________\n" << endl;
	}
}

int main( int argc, char* argv[] )
{
	cout << "-----------------------------Testing chen Formulation----------------------------------" << endl;
	test_bot_chen_hand(1000);
	cout << "-------------------------------End Chen Testing----------------------------------------" << endl;
	return 0;
}





