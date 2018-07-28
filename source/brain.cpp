#include "brain.h"

Brain::Brain() { game = nullptr; }

// TOOD:: needs testing
bool Brain::assessTable(Player *bot,
                        poker_states state) {    /* TODO:: Implement assessTable function to switch through each state of the game */
    switch (state) {
        case FLOP:
            assessFlopScenario(bot);
            break;
        case DEAL_HOLE_CARDS:
            break;
        case PREFLOP:
            break;
        case ROUND_OF_BETS:
            break;
        case DEAL_FLOP:
            break;
        case DEAL_TURN:
            break;
        case TURN:
            break;
        case DEAL_RIVER:
            break;
        case RIVER:
            break;
        case SHOWDOWN:
            break;
        default:
            break;
    }
    return true;
}

bool Brain::assessTable(const Cards &cds) {
    if (cds.size() != 5) {
        LOG << "Brain::assessTable(): need 5 cards present, exiting ..." << "\n";
        return false;
    }


    return false;
}

Brain::Brain(Poker *pkr) : game(pkr) {

}

//--------------------------------------------------JayBrain----------------------------------------
JayBrain::JayBrain() : Brain() {
    // will need to call init() with a poker game when it becomes available
}

JayBrain::JayBrain(TexasHoldem *pkr) : Brain(pkr) {}

void JayBrain::init(TexasHoldem *pkr) {
    // txGame should be on heap, from outside this class, point to game at hand
    game = pkr;
}

// TODO:: needs testing 
bool JayBrain::assessFlopScenario(Player *bot) {
    /*
        1. analyze flop situation
        2. find out what hand we are closest to and use that hand to go for
        3. calculate pot odds
        4. calculate card odds
        5. use card and pot odds to inform decision
        6. take into consideration actions of other players in game
        7. make bet based on data collected
    */
    size_t numCardsDealt = 52 - game->gameDeck.numCardsLeft();
    int outs;   // number of cards left in the deck that contribute to achieving a best hand
    Card hole1 = bot->holeCards.first;
    Card hole2 = bot->holeCards.second;
    if (thGame->getTableCards().size() != 3) {
        LOG << __FUNCTION__ << "(): " << E << "Error --> tableCards.size() != 3, exiting" << END;
        return false;
    }
    // first we see if we have a valid hand off the flop
    auto it = bot->hand.cards.begin();
    bot->hand.cards.insert(it, thGame->getTableCards().begin(), thGame->getTableCards().end());

    if (bot->hand.cards.size() != 5)
        return false;

    // here we can run through and determine what hand type we have based on the five cards out
    bot->setCurrentBestHand( assessment::findHandStrength(bot->hand.cards) );

    // find odds of achieving a better hand or acheiving a hand from nothing on the turn
    return true;
}
