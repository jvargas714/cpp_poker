#include "TexasHoldem.h"

using namespace std;

int main( int argc, char** argv )
{

	// set number of test games through arg to command line 
	// int numGames = atoi(argv[1]);

	TexasHoldem game( 50, 100, 3750 );

	string name1( "Player 3" );
	string name2( "Player 4" );

	game.enterGame( "Player 3", 3750 );
	game.enterGame( "Player 4", 3750 );

	Player* p1 = &game.players[ 0 ];
	Player* p2 = &game.players[ 1 ];
	Player* p3 = &game.players[ 2 ];
	Player* p4 = &game.players[ 3 ];

	//cout << "Game will play for " << numGames << " hands" << endl;
	int numGames;
	cout << "Enter how many games should be simulated: " << endl;
	cin >> numGames;

	for ( int i = 1; i <= numGames; ++i )
	{
		game.dealHands();

		game.betSmallBlind( p1 );
		game.betBigBlind( p2 );
		game.playerBet( p3, 0 );
		game.playerBet( p4, 0 );

		game.gameStatus();

		game.changeBlinds( 50, 100 );

		game.dealFlop();

		game.playerBet( p1, 100 );
		game.playerBet( p2, 150 );
		game.playerBet( p3, 250 );
		game.playerBet( p4, 400 );

		game.dealTurn();
		game.dealRiver();
		game.showTableCards();

		game.findHand( p1 );
		game.findHand( p2 );
		game.findHand( p3 );
		game.findHand( p4 );

		game.findWinner();    // debug here

		/*
		if(p1->handStrength == p2->handStrength){
			cout << "Hands are tied no winner" << endl;
		}

		else if(p1->handStrength > p2->handStrength){
			
			cout << p1->name << " wins hand!!" << endl;
			game.collectPot(p1);
		}

		else{
			cout << p2->name << " wins hand!!" << endl;
			game.collectPot(p2);
		}*/

		game.resetHand( i );    // reset table for another hand
	}

	return 0;
}
