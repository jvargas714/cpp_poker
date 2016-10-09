#ifndef TEXASHOLDEM_H_INCLUDED
#define TEXASHOLDEM_H_INCLUDED

#include <iostream>
#include <set>
#include <cstdlib>
#include "card.h"
#include "hand_weight.h"
#include "poker.h"
#include "json/json.h"
#include "json/json-forwards.h"

/*
	Author:
		Jay Vargas

	Description:
		Texas Holdem Class inherits Poker abstract class

	Methods:
		1. [ctor] TexasHoldem()  ***
			- by default creates a game with 2 players Jay and Kelsey
			- assigns $1500 to each player
			- initial small and big blinds will be 10, 20 respectively

		   [ctor] TexasHoldem(int smallBlind, int bigBlind, int cash)   ***
		   	- start a game specifying small, big blind as well as players
		   		starting cash

   		   [ctor] TexasHoldem(std::initializer_list<std::pair<string, int>>)   ***
   		   	- start game with list of players specified by std::pair<name, cash>
   		   	- initial small and big blinds will be 10, 20 respectively

   		   [ctor] TexasHoldem(std::initializer_list<string> names)   ***
   		   	- start a game by specifying list a names of players
   		   	- starting cash amount will be default $1500
   		   	- initial small and big blinds will be 10, 20 respectively

		2. void showHand(const Player*)   ***
			- displays player's status
			- prints hand to console

		3. void dealHands()
			- deals 2 cards to each player in the game
			- the card dealt is removed from the game deck

		4. void findHand(const Player*)
			- copies hole cards
			- creates ArrayList of 7 Cards (2 hole cards and 5 Community cards)
			- creates a Set<Set>> of cards where inner set is 5 cards
				a. holds 21 possible hands (combo where order does not matter)
			- from the set the best hand is found and assigned a weight to the
				player's handStrength member

		5. void dealFlop()
			- deals 3 community cards to the table (deals flop)

		6. void dealTurn()
			- deals turn card to table

		7. void dealRiver()
			- deals river card to table (5th and final community card)

		8. void showTableCards() const
			- prints community cards to the console
			- really used for testing purposes

		9. void playerBet(const Player*, int amt)
			- player makes bet & pot grows by amt

		10. void betSmallBlind(const Player*)
			- player makes small blind bet

		11. void betBigBlind(const Player*)
			- player mPakes big blind bet

		12. void resetHand()
			- resets table for next hand of poker

		13. bool findWinner()
			- creates a list of players in order by their hand
				strengths
			- singles out player with highest hand strength
				* sorted players using a hand comparator
			- loops through players looking for a tie of highest
				hand
			- if highest players is one 1 in size then we have a
				winner
				* else sort out true winner by finding a high card
					or whatever it may be
				* could end up being a tie

		14. void listPlayerStatus()
			- prints to console all players with cash amount and name
			- private method

		15. cardSet comboCards() const;
			- takes table cards and player's cards and creates a set of 21 sets
				of 5 card combination for best hand evaluation

	Global Functions:
		1. intComb comb(int n, int k)
			- return vector of vector<int>
				containing all k long combinations of a n long vector(0....n-1);

		2. long nCr(int n, int k)
			- n choose k Combinations Order does not matter

		3. long fact(n)
			- returns n factorial n!

	Types:
		cardSuperSet --> std::set<std::set<Card>>
		intComb --> vector<vector<int>>
*/
// debug cfg file 
const string CFG_FILE = "/home/likewise-open/G3TI/jay.vargas/Documents/Dev/Poker/poker/config/TXHldm.json";
typedef std::vector<std::vector<Card>> cardSuperVector;	// card vector embedded in card set for combo Method
typedef vector<vector<int>> intComb;

class TexasHoldem : public Poker
{

private:
	int pot, smallBlind, bigBlind, gameId;
	// flags 
	bool findHandFlag;

public:
	Cards tableCards;
	uint32_t dealerIndex;

	TexasHoldem();
	TexasHoldem(std::string& cfg_path);
	TexasHoldem(int smallBlind, int bigBlind, int cash);
	TexasHoldem(std::initializer_list<string>);
	TexasHoldem(std::initializer_list<std::pair<string, int>>);
	void showHand(const Player*) const;						// pure virtual in poker
	void dealHands();										// pure virtual in poker
	void findHand(Player*);									// pure virtual in poker
	void changeBlinds(int sm, int big);
	void dealFlop();
	void dealTurn();
	void dealRiver();
	void playerBet(Player*, int amt);
	void betSmallBlind(Player*);
	void betBigBlind(Player*);
	cardSuperVector comboCards(const Player*) const;
	void showTableCards() const;
	void resetHand(int);
	void collectPot(Player*);
	void gameStatus() const;
	void gameOverview() const; 
   	virtual void findWinner();

private:
	Player* findPlayer(const string name);
	void assign_dealer();
	void rotateDealer();
};


long nCr(int n, int k);
long fact(int n);
intComb comb(int n, int k);
void debugPrintVect(vector<int>);

#endif
