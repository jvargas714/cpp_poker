/*
 * Created by Jay on 6/15/2015
 * Hand Rankings: High to low
 * 1. Royal Flush
 * 2. Straight Flush
 * 3. Four of a Kind
 * 4. Full House
 * 5. Flush
 * 6. Straight
 * 7. Three of a Kind
 * 8. Two Pair
 * 9. One Pair
 * 10. High Card
 */

#include <iostream>
#include "poker.h"

Poker::Poker()
// creates new game defaulting to 0 players with default cash $1500
        : players({Player("Player 1", 1500), Player("Player 2", 1500)}), numPlayers(2), gameDeck() {}

Poker::Poker(int cash)
// creates new game defaulting to 2 players with specified starting cash
        : players({Player("Player 1", cash), Player("Player 2", cash)}), gameDeck() {
    numPlayers = 2;
}

Poker::Poker(std::initializer_list<std::string> names)
/*
 * Default cash: $1500
 */
        : players(), gameDeck() {
    for (const std::string &name : names) {
        LOG << __FUNCTION__ << " name " << name << "\n";
        players.emplace_back(Player(name, 1500));
    }
    numPlayers = static_cast<uint32_t>(players.size());
}

Poker::Poker(std::initializer_list<std::pair<std::string, int> > list)
        : players(), gameDeck() {
    /*
     * Specify list of pairs player : cash
     */
    using std::pair;
    for (pair<std::string, int> nameCashPair : list) {
        players.push_back(Player(nameCashPair.first, nameCashPair.second));
    }
    numPlayers = list.size();
}

Poker::~Poker()=default;

void Poker::enterGame(std::string name, int cash) {
    players.emplace_back(Player(name, cash));
    LOG << name << " has entered the game with $" << cash << "\n";
    numPlayers++;
    LOG << "Number of players: " << numPlayers << "\n";
}

void Poker::enterGame(std::initializer_list<std::pair<std::string, int>> players) {
    for (std::pair<std::string, int> player : players) {
        this->players.emplace_back(
                Player(player.first, player.second)
        );
    }
}

std::vector<Player>::iterator Poker::getPlayerIterator(const int& index) {
    LOG << "finding player via index: " << index << END;
    if (index < 0 || index > players.size()-1) throw poker_error::FindPlayerError();

    // returns a reference to a player's reference by index
    return players.begin()+index;
}

std::vector<Player>::iterator Poker::getPlayerIterator(const std::string &name) {
    LOG << "finding player via name: " << name << END;
    for (auto it = players.begin(); it != players.end(); it++)
        if (it->name == name)
            return it;
    throw poker_error::FindPlayerError();
}
