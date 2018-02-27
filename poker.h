//
// Created by Jay on 6/15/2015.
/*
 * Abstract poker class with some pure virtual functions and general poker functions
 *      Considerations:
 *          1. maybe return a vector for hands or pointer to array
 *          2. consider making hasHAnd() methods to be at least protected
 *
 * Usage:
 *  1. for getHand() methods ie. getPair, getStraight ... only call these methods
 *      if the hasPair, hasStraight bool methods have been met, or an exception will
 * 		be thrown
 *
 * 	TODO:: Need to refactor this class, set data members private, remove pointers and unecessary
 *
 */

#ifndef POKER_H_INCLUDED
#define POKER_H_INCLUDED

#include "player.h"
#include "card.h"
#include "deck.h"
#include "poker_error.h"
#include "poker_types.h"
#include <algorithm>
#include <unordered_map>

/*
    Poker(std::initializer_list<std::pair<string, int> >)
        ** players specified by std::pair<name, cash)

    Poker(int cash)
        ** 2 players starting with specified cash

    Poker(std::initializer_list<string> names)
        ** constructor where players are specified and cash they start with

    enterGame(...)
        ** various methods to enter players in different ways

    enterGame(std::initializer_list<std::pair<string, int> > players) 
        ** enters list of players, specifying name and cash in a std::pair<>

    dealCard(Player*, Card&&)
        ** can specify what kind of card to deal out, mainly for testing

    void dealCard(Player*)
        ** deals card of top of deck, that card in removed from Deck.deck

    vector<int> dupRankInd(const Player*) const
        ** returns index in hand of matched ranks

    int* getRankInd(const Player*) const
        ** returns rank index of each card in player->hand

    vector<string> getHandSuits(const Player*) const
        ** returns suits of all cards in hand
*/
class Poker
{
public:
	std::vector<Player> players;
	uint32_t numPlayers = 0;
	Deck gameDeck;
	Poker();
	Poker( std::initializer_list<std::pair<std::string, int> > );
	explicit Poker( int cash );
	Poker( std::initializer_list<std::string> names );
	virtual ~Poker();
	void enterGame( std::string name, int cash );
	void enterGame( std::initializer_list<std::pair<std::string, int> > players );
	void dealCard( Player*, Card&& );
	void dealCard( Player* );
	Player& getPlayerRef( int index );
	Player& getPlayerRef( std::string& name );
	/*
		This set of methods are has and get methods that return a bool and vector<Cards>
			--> has___() returns true if and only if the respective hand type
					is the best hand present in the players hand at the time
			--> get__() returns typedef Cards if has__() for that respective
					handtype returns true, if get__() is called when has__() returns
					false a runtime_error exception is thrown (HandIDError) identifying
					the hand type in question
	*/
	bool hasPair( const Player* ) const;
	Cards getPair( const Player* ) const;
	bool hasTwoPair( const Player* ) const;
	Cards getTwoPair( const Player* ) const;
	bool hasTrips( const Player* ) const;
	Cards getTrips( const Player* ) const;
	bool hasFourOfAKind( const Player* ) const;
	Cards getFourOfAKind( const Player* ) const;
	bool hasStraight( const Player* ) const;
	Cards getStraight( const Player* ) const;
	bool hasFlush( const Player* ) const;
	Cards getFlush( const Player* ) const;
	bool hasFullHouse( const Player* ) const;
	Cards getFullHouse( const Player* ) const;
	bool hasStraightFlush( const Player* ) const;
	Cards getStraightFlush( const Player* ) const;
	bool hasRoyalFlush( const Player* ) const;
	Cards getRoyalFlush( const Player* ) const;
	/*==============End of get__() and has__() methods=====================*/

	/*pure virtual methods*/
	virtual void showHand( const Player* ) const = 0;
	virtual void dealHands() = 0;
	virtual void findHand( Player* ) = 0;
protected:
	std::vector<int> dupRankInd( const Player* ) const;
	int* getRankInd( const Player* ) const;
	std::vector<std::string> getHandSuits( const Player* ) const;
};
#endif //POKER_H_INCLUDED
