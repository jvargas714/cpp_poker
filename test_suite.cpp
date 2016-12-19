#include "master_poker.h"

#define TEST_CNT 5
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
		bot.hand.push_back( cd );
		bot.hand.push_back( cd2 );
		bot.computeStartingPower();
		bot.getBotStatus();
		bot.setStartingPower( 0 );
		bot.hand.clear();
		cout << "___________________________________________________________________________________\n" << endl;
	}
}

void test_bot_ctors()
{
	bot_player bot;
	bot_player bot2( "McDousherstein", 2500 );

	cout << "-----bot-------" << endl; 
	cout << bot << endl;
	cout << endl; 

	cout << "------bot2-----" << endl; 
	cout << bot2 << endl; 
	cout << endl;
}

int main( int argc, char* argv[] )
{
	cout << "-----------------------------Testing chen Formulation----------------------------------" << endl;
	test_bot_ctors();
	cout << "---------------------------------------------------------------------------------------" << endl;

	cout << "-----------------------------Testing chen Formulation----------------------------------" << endl;
	test_bot_chen_hand( TEST_CNT );
	cout << "-------------------------------End Chen Testing----------------------------------------" << endl;
	return 0;
}





