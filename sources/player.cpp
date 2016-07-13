//
// Created by jay on 6/6/15.
//
#include "player.h"
#include "hand_weight.h"
#include <iostream>

using std::cout;
using std::endl;


Player::Player()
        :smallBlind_bet(false), bigBlind_bet(false), dealer(false),
		 cash(1500), handStrength(0), highCardRnk(0)
{
//    string plyrName("Player 1");
//    string tmp("1");
//    plyrName.append(tmp);
//    name = plyrName;
    name = "Player 1";
}


Player::Player(string name, int cash)
        : smallBlind_bet(false), bigBlind_bet(false), dealer(false),
		  cash(cash), handStrength(0), highCardRnk(0), name(name)
{}

Player::~Player() {
    // cout << "Player: " << name << " destroyed" << endl;
}


void Player::namePlayer(string name) {

    string originalName(name);
    this->name = name;

    cout << originalName << " was named to " << this->name << endl;

}

int Player::bet(int amt) {
    cout << name << " bet $" << amt << endl;
    cash -= amt;
    return 0;
}

int Player::bet(){
	// wrapper function for bet(int)
	// socket interface from user on how much to bet
	// after value is read in calls bet(int) to actually place the bet0
	return 0;
}

void Player::collectPot(int amt) {
    cash += amt;
    cout << name << " collected $" << amt << ", new balance: $" << cash << "\n" << endl; 
}

void Player::setBestHand(vector<Card> &cards) {
    // copys over cards to best hand, then deletes cards
    int i = 0;

    for(Card cd: cards){
        bestHand[i] = cd;
        i++;
    }
    cout << name << " has set best hand " << endl;
}

void Player::setHandStrength(const string &handType, int &rank) {
    handStrength = HandMapper::handMap.at(handType)[rank];
}

void Player::clearHand() {
    hand.clear();
}

void Player::clearHandStrength() {
    handStrength = 0;
}

void Player::reset(){
    clearHand();
    clearHandStrength();
    bestHand.clear();
    highCardRnk = 0;
}

bool Player::cmpCash(const Player &plyr1, const Player &plyr2) {
    // compare function for various sorting algos
     return (plyr1.cash < plyr2.cash);
}

bool Player::operator == (const Player& plyr) const{
    return cash == plyr.cash;
}

bool Player::operator < (const Player& plyr) const{
    return cash < plyr.cash;
}

bool Player::operator > (const Player& plyr) const{
    return cash > plyr.cash;
}

std::ostream& operator << (std::ostream &os, const Player &plyr){
    os << "Name: "<< plyr.name << "\nBalance: " << plyr.cash << endl;
    return os;
}


// impement Player hand comparator functor 
bool HandComparator::operator()(const Player& p1, const Player& p2) const {

    return p1.handStrength < p2.handStrength;
}

// implement player high card rank functor 
bool HighCardRnkComparator::operator()(const Player& p1, const Player& p2) const{

    return p1.highCardRnk < p2.highCardRnk;
}
