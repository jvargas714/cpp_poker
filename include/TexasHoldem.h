#ifndef TEXASHOLDEM_H_INCLUDED
#define TEXASHOLDEM_H_INCLUDED

#include <iostream>
#include <set>
#include <cstdlib>
#include <cstdint>
#include "poker.h"
#include "json.h"
#include "json-forwards.h"

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
const std::string CFG_FILE = "../config/TXHldm.json";

class Poker;

class TexasHoldem : public Poker {
private:
    uint32_t pot, smallBlind, bigBlind, gameId;
    Cards tableCards;
public:
    uint32_t dealerIndex;

    TexasHoldem();
    friend std::ostream& operator<<(std::ostream& g1, TexasHoldem&);

    TexasHoldem& operator=(TexasHoldem);

    bool operator==(const TexasHoldem&) const;

    explicit TexasHoldem(const std::string &cfg_path);

    TexasHoldem(uint32_t smallBlind, uint32_t bigBlind, uint32_t cash);

    TexasHoldem(std::initializer_list<std::string>);

    TexasHoldem(std::initializer_list<std::pair<std::string, int>>);

    TexasHoldem(const TexasHoldem &&) noexcept;

    TexasHoldem(const TexasHoldem &) noexcept;

    ~TexasHoldem()override=default;

    void showHand(const Player *) const override {};

    void dealHands() override;

    void findHand(Player *) override;

    void changeBlinds(uint32_t sm, uint32_t big);

    void dealFlop();

    void dealTurn();

    void dealRiver();

    void playerBet(Player *, int amt);

    void betSmallBlind(Player *);

    void betBigBlind(Player *);

    cardSuperVector comboCards(const Player *) const;

    cardSuperVector permuteCards(const Player& plyr) const;

        void showTableCards() const;

    void resetHand();

    void collectPot(Player *);

    void gameStatus() const;

    void gameOverview() const;

    inline uint32_t getSmallBlind() const noexcept { return smallBlind; }

    inline uint32_t getBigBlind() const noexcept { return bigBlind; }

    inline uint32_t getGameId() const noexcept { return gameId; }


    void findWinner() override;

    void calculateHands();

    inline void setBigBlind(uint32_t val) noexcept { bigBlind = val; }

    inline void setSmallBlind(uint32_t val) noexcept { smallBlind = val; }

    inline std::vector<Player>::iterator getPlayerIterator(const int& index) override { return Poker::getPlayerIterator(index); }

    inline std::vector<Player>::iterator getPlayerIterator(const std::string& name) override { return Poker::getPlayerIterator(name); }

    inline Cards& getTableCards() { return tableCards; }
    inline void setPot(uint32_t amt) { pot = amt; }
private:
    Player *findPlayer(const std::string &name);

    void assign_dealer();

    void rotateDealer() override;
};

uint64_t nCr(uint64_t n, uint64_t k);

uint64_t fact(uint64_t n);

intComb comb(uint64_t n, uint64_t k);

// operator overloads
std::ostream& operator<<(std::ostream& os, TexasHoldem& game);

// DEBUG
void debugPrintVect(std::vector<int>);

#endif
