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

    std::vector<int> getRankInd(const Player*) const
        ** returns rank index of each card in player->hand

    vector<string> getHandSuits(const Player*) const
        ** returns suits of all cards in hand
*/
class Poker {
public:
    std::vector<Player> players;
    uint32_t numPlayers = 0;
    Deck gameDeck;

    Poker();
    Poker(std::initializer_list<std::pair<std::string, int> >);
    explicit Poker(int cash);
    Poker(std::initializer_list<std::string> names);
    virtual ~Poker();
    void enterGame(std::string name, int cash);
    void enterGame(std::initializer_list<std::pair<std::string, int> > players);
    virtual std::vector<Player>::iterator getPlayerIterator(const int& index);
    virtual std::vector<Player>::iterator getPlayerIterator(const std::string &name);

    /*pure virtual methods*/
    virtual void showHand(const Player *) const = 0;
    virtual void dealHands() = 0;
    virtual void findHand(Player *) = 0;
};
#endif //POKER_H_INCLUDED
