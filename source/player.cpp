//
// Created by jay on 6/6/15.
//
#include "player.h"
#include "hand_weight.h"

Player::Player()
        : smallBlind_bet(false), bigBlind_bet(false), dealer(false),
          cash(1500), hand(), highCardRnk(0), name("Player") {
    name = "Player";
}


Player::Player(std::string name, int cash)
        : smallBlind_bet(false), bigBlind_bet(false), dealer(false),
          cash(cash), hand(), highCardRnk(0), name(name) {}


Player::~Player() {}


void Player::namePlayer(std::string name) {
    std::string originalName(name);
    this->name = name;
    LOG << originalName << " was named to " << this->name << std::endl;
}

int Player::bet(int amt) {
    LOG << name << " bet $" << amt << std::endl;
    cash -= amt;
    return 0;
}

// TODO:: should be implmented
int Player::bet() {
    // wrapper function for bet(int)
    // socket interface from user on how much to bet
    // after value is read in calls bet(int) to actually place the bet0
    return 0;
}

void Player::collectPot(int amt) {
    cash += amt;
    LOG << name << " collected $" << amt << ", new balance: $" << cash << "\n" << std::endl;
}

void Player::setHandStrength(const std::string &handType, int &rank) {
    hand.strength = HandMapper::handMap.at(handType)[rank];
}

void Player::clearHand() {
    clearHandStrength();
    hand.cards.clear();
    hand.hand_str = "";
    hand.type = HAND_TYPE::NONE;
}

void Player::clearHoleCards() {
    holeCards.first = Card(-1, -1);
    holeCards.second = Card(-1, -1);
}

void Player::clearHandStrength() {
    hand.strength = 0;
}

void Player::reset() {
    clearHand();
    clearHandStrength();
    highCardRnk = 0;
}

bool Player::cmpCash(const Player &plyr1, const Player &plyr2) {
    // compare function for various sorting algos
    return (plyr1.cash < plyr2.cash);
}

bool Player::operator==(const Player &plyr) const {
    return cash == plyr.cash;
}

bool Player::operator<(const Player &plyr) const {
    return cash < plyr.cash;
}

bool Player::operator>(const Player &plyr) const {
    return cash > plyr.cash;
}

std::ostream &operator<<(std::ostream &os, const Player &plyr) {
    os << "Name: " << plyr.name << "\nBalance: " << plyr.cash << "\n";
    return os;
}

bool Player::isValidHand() const {
    return !(holeCards.first.rankIndex<0||holeCards.second.rankIndex<0||
                    holeCards.first.suitIndex<0||holeCards.second.suitIndex<0);
}

// impement Player hand comparator functor
bool HandComparator::operator()(const Player &p1, const Player &p2) const {
    return p1.hand.strength < p2.hand.strength;
}

// implement player high card rank functor
bool HighCardRnkComparator::operator()(const Player &p1, const Player &p2) const {
    return p1.highCardRnk < p2.highCardRnk;
}
