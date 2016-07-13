#include "deck.h"
#include <iostream>
#include <algorithm>        // std::random_shuffle()

using std::cout;
using std::endl;

Deck::Deck()
{
    /*
        forms a 52 card vector representing the deck
    */

    for(int i = 0; i<13; i++){

        for(int j = 0; j < 4; j++){
            Card card(i,j);
            deck.push_back(card);
        }
    }

    shuffleDeck();
}


Deck::~Deck(){}


void Deck::shuffleDeck()
{ // shuffle deck according to rand num gen
    static int offset = 0;
	std::srand(time(0) + offset);
    offset += 57; 
    std::random_shuffle(deck.begin(), deck.end());
}


void Deck::displayCard(uint8_t &index)
{// prints to screen card at index
    cout << deck[index];
}


void Deck::resetDeck()
{ // clears deck, fills deck with new cards and shuffles
    deck.clear();               // clears vector leaving it with a size of 0

    for(int i = 0; i<13; i++){

        for(int j = 0; j < 4; j++){
            Card card(i,j);
            deck.push_back(card);
        }
    }
    shuffleDeck();

    cout << "Deck reset and shuffled!!" << endl;
}


Card Deck::dealCard()
{
    /**
     * burns a card and deals
     */
    Card tmp(deck.back());              //make copy to return
    deck.pop_back();
    return tmp;
}


size_t Deck::numCardsLeft() const
{
/*
 *  returns amount of cards left in the deck
 */
    return deck.size();
}
