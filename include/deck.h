#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#include "card.h"
#include <vector>
#include <cstdint>

class Deck {
private:
    std::vector<Card> deck;
public:
    Deck();

    ~Deck();

    void shuffleDeck();                     // will delete original deck and create a new one
    void displayCard(uint8_t &index);        // prints card located @ index
    void resetDeck();                       // delete deck and create a new deck and shuffle use erase method
    Card dealCard();                        // deals card off the top of the d3ck
    size_t numCardsLeft() const;           // returns number of cards left in deck
};

#endif // DECK_H_INCLUDED
