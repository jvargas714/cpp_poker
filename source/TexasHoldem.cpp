#include <random>
#include "TexasHoldem.h"
#include "hand_weight.h"
#include "poker_error.h"
#include "assessment.h"

TexasHoldem::TexasHoldem()
        : Poker(), pot(0), smallBlind(10), bigBlind(20), gameId(999) {
    // Constructor that inits from configuration
    LOG << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n" << std::endl;
    LOG << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" << std::endl;
    for (auto &p: players) {
        LOG << p << std::endl;
    }
    assign_dealer();
}

TexasHoldem::TexasHoldem(const std::string &loadPath)
        : Poker(), pot(0) {

    players.clear();

    // Init game from supplied config file
    LOG << "Game init via " << loadPath << std::endl;
    std::ifstream cfgFile(loadPath);

    if (!cfgFile.is_open()) {
        LOG << "ERROR :: cfg file not open, exiting..." << END;
        throw poker_error::CfgLoadError("Error loading config file: " + loadPath);
    }

    Json::Value root;
    cfgFile >> root;
    LOG << "cfg file read:: " << root << END;
    Json::Value def_val(-1);
    gameId = root["game"]["id"].asUInt();
    LOG << "game CFG::Game Id: " << gameId << END;

    std::string key = "small_blind";
    def_val = 10;
    smallBlind = root["game"].get(key, def_val).asUInt();
    LOG << "game CFG::smallBlind:: " << smallBlind << std::endl;

    key = "big_blind";
    def_val = 20;
    bigBlind = root["game"].get(key, def_val).asUInt();
    LOG << "game CFG::bigBlind:: " << bigBlind << std::endl;

    Json::Value player_array;
    player_array = root["game"]["players"];
    if (!player_array.isArray()) {
        // take some sort of contingency
        std::cerr << "Error reading players from configurations, exiting" << std::endl;
        return;
    }
    Json::Value::iterator it = player_array.begin();
    std::string player_name;
    numPlayers = root["game"]["number_of_players"].asUInt();

    while (it != player_array.end()) {
        player_name = (*it)["name"].asString();
        Player plyr(player_name, (*it)["starting_cash"].asInt());
        players.push_back(plyr);
        LOG << "Created player: " + player_name << std::endl;
        it++;
    }

    def_val = true;
    key = "random_dealer";
    if(!root["game"].get(key, def_val).asBool()) {
        assign_dealer();
    } else {
        def_val = 0;
        key = "dealer_index";
        dealerIndex = root["game"].get(key, def_val).asUInt();
    }

}

TexasHoldem::TexasHoldem(uint32_t smallBlind, uint32_t bigBlind, uint32_t cash)
        : Poker(cash), pot(0), smallBlind(smallBlind), bigBlind(bigBlind), gameId(999) {
    LOG << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n" << std::endl;
    LOG << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" << std::endl;
    for (auto &p: players) {
        LOG << p << std::endl;
    }
    assign_dealer();
}

TexasHoldem::TexasHoldem(std::initializer_list<std::pair<std::string, int>> list)
        : Poker(list), pot(0), smallBlind(10), bigBlind(20), gameId(999) {
    LOG << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n" << std::endl;
    LOG << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" << std::endl;
    for (auto &p: players) {
        LOG << p << std::endl;
    }
    assign_dealer();
}

TexasHoldem::TexasHoldem(std::initializer_list<std::string> names)
        : Poker(names), pot(0), smallBlind(10), bigBlind(20), gameId(999) {
    LOG << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"
          << END;
    LOG << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n"
          << END;
    for (auto &p: players) {
        LOG << p << END;
    }
    assign_dealer();
}

TexasHoldem::TexasHoldem(const TexasHoldem &&game) noexcept
        : Poker()  {
    this->smallBlind = game.smallBlind;
    this->bigBlind = game.bigBlind;
    this->gameDeck = game.gameDeck;
    this->players = game.players;
    this->gameId = game.gameId;
    this->dealerIndex = game.dealerIndex;
    this->numPlayers = game.numPlayers;
    this->pot = game.pot;
    this->tableCards = game.tableCards;
}

TexasHoldem::TexasHoldem(const TexasHoldem &game) noexcept
        : Poker() {
    this->smallBlind = game.smallBlind;
    this->bigBlind = game.bigBlind;
    this->gameDeck = game.gameDeck;
    this->players = game.players;
    this->gameId = game.gameId;
    this->dealerIndex = game.dealerIndex;
    this->numPlayers = game.numPlayers;
    this->pot = game.pot;
    this->tableCards = game.tableCards;
}

void TexasHoldem::dealHands() {
        for (auto &player : players)
            player.holeCards.first = gameDeck.dealCard();
        for (auto& player : players)
            player.holeCards.second = gameDeck.dealCard();
    LOG << "cards dealt out to players.... \n\n" << END;
}

void TexasHoldem::changeBlinds(uint32_t sm, uint32_t big) {
    smallBlind = sm;
    bigBlind = big;
    LOG << "Small blind has been increased to $" << smallBlind << END;
    LOG << "Big blind has been increased to $" << bigBlind << "\n" << END;
}

void TexasHoldem::showTableCards() const {
    if (tableCards.empty()) {
        LOG << "No table cards ....\n\n " << std::endl;
    } else {
        LOG << "Table Cards: " << std::endl;
        for (auto &cd : tableCards) {
            LOG << cd << std::endl;
        }
    }
}

void TexasHoldem::dealFlop() {
    gameDeck.dealCard();  // burn card
    for (int i = 0; i < 3; i++) {
        tableCards.push_back(gameDeck.dealCard());
    }
    LOG << "Flop has been dealt....\n" << std::endl;
}

void TexasHoldem::dealTurn() {
    gameDeck.dealCard();  // burn card
    tableCards.push_back(gameDeck.dealCard());
    LOG << "Turn card has been dealt....\n" << std::endl;
}

void TexasHoldem::dealRiver() {
    gameDeck.dealCard();  // burn card
    tableCards.push_back(gameDeck.dealCard());
    LOG << "River card has been dealt...\n" << std::endl;
}

void TexasHoldem::playerBet(Player *plyr, int amt) {
    plyr->bet(amt);
    pot += amt;
    LOG << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::betSmallBlind(Player *plyr) {
    plyr->bet(smallBlind);
    pot += smallBlind;
    LOG << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::betBigBlind(Player *plyr) {
    plyr->bet(bigBlind);
    pot += bigBlind;
    LOG << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::findHand(Player *plyr) {
    /*
        1. id player
        2. copy hole cards
        3. create list of 7 cards 2 hole and 5 table cards
        4. create sets of cards where inner set is 5 cards, will hold 21
            possible combinations using a combo function
        5. from set will find the best hand and assign a int weight to player's
            handStrength by looping through all possible combos
        6. assigns a high card value to break ties

        TODO: need to insert logic to differentiate when assigning highCardRnk
                the rank cannot be the same as the rank of the matched hand 3 of a kind, pair
                two pair etc etc ...
                ie if a player has a pair of 8s and 8s are the highest card on the table
                the high card cannot be 8, need to fix this, 8/21/2015 @2236
    */
    if (tableCards.size() != 5 || !plyr->isValidHand()) {
        LOG << E << " table is not in a valid state, exiting...." << END;
        throw poker_error::HandIDError("Error--> TexasHoldem:findHand():: 7 cards not present");
    }
    Cards allCards = tableCards;
    Cards bestHand;
    cardSuperVector possHands = comboCards(plyr);
    Card hole1 = plyr->holeCards.first;
    Card hole2 = plyr->holeCards.second;
    allCards.push_back(hole1);
    allCards.push_back(hole2);
    sort(allCards.begin(), allCards.end());
    plyr->clearHand();
    std::string message("error");

    for (Cards &possHand: possHands) {
        plyr->hand.cards = possHand;
        if (assessment::hasRoyalFlush(plyr->hand.cards)) {
            int tmp = HandMapper::handMap.at("royalFlush")[0];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->highCardRnk = 0;
                plyr->hand.type = HAND_TYPE::ROYAL_FLUSH;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::ROYAL_FLUSH);
                message = "a Royal Flush";
            }
        } else if (assessment::hasStraightFlush(plyr->hand.cards)) {
            int highCardRank = plyr->hand.cards[4].rankIndex;
            int tmp = HandMapper::handMap.at("straightFlush")[highCardRank];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->highCardRnk = 0;
                plyr->hand.type = HAND_TYPE::STRAIGHT_FLUSH;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::STRAIGHT_FLUSH);
                message = "a Straight Flush";
            }
        } else if (assessment::hasFourOfAKind(plyr->hand.cards)) {
            int cardRnk = assessment::getFourOfAKind(plyr->hand.cards)[0].rankIndex;
            int tmp = HandMapper::handMap.at("fourOfAKind")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::FOUR_OF_A_KIND);
                plyr->hand.type = HAND_TYPE::FOUR_OF_A_KIND;
                if (cardRnk == allCards[6].rankIndex) {
                    plyr->highCardRnk = allCards[2].rankIndex;  // c0 c1 c2 c3 c4 c5 c6-->
                } else {
                    plyr->highCardRnk = allCards[6].rankIndex;
                }
                message = "a Four of a Kind";
            }
        } else if (assessment::hasFullHouse(plyr->hand.cards)) {
            Cards fullHouse = assessment::getFullHouse(plyr->hand.cards);
            int cardRnk;
            if (std::count(fullHouse.begin(), fullHouse.end(), fullHouse[0]) == 3) {
                cardRnk = fullHouse[0].rankIndex;
            } else {
                cardRnk = fullHouse[4].rankIndex;
            }
            int tmp = HandMapper::handMap.at("fullHouse")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->highCardRnk = 0;
                plyr->hand.type = HAND_TYPE::FULL_HOUSE;
                message = "a Full House";
            }
        } else if (assessment::hasFlush(plyr->hand.cards)) {
            Cards flush = assessment::getFlush(plyr->hand.cards);
            int cardRnk = flush[4].rankIndex;
            int tmp = HandMapper::handMap.at("flush")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->highCardRnk = 0;
                plyr->hand.type = HAND_TYPE::FLUSH;
                message = "a Flush";
            }
        } else if (assessment::hasStraight(plyr->hand.cards)) {
            int cardRnk = assessment::getStraight(plyr->hand.cards)[4].rankIndex;
            int tmp = HandMapper::handMap.at("straight")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->highCardRnk = 0;
                plyr->hand.type = HAND_TYPE::STRAIGHT;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::STRAIGHT);
                message = "a Straight";
            }
        } else if (assessment::hasTrips(plyr->hand.cards)) {
            int cardRnk = assessment::getTrips(plyr->hand.cards)[0].rankIndex;
            int tmp = HandMapper::handMap.at("trips")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::TRIPS);
                plyr->hand.type = HAND_TYPE::TRIPS;
                if (cardRnk == allCards[6].rankIndex) {
                    plyr->highCardRnk = allCards[3].rankIndex;
                } else {
                    plyr->highCardRnk = allCards[6].rankIndex;
                }
                message = "a Three of a Kind";
            }
        } else if (assessment::hasTwoPair(plyr->hand.cards)) {
            Cards twoPair = assessment::getTwoPair(plyr->hand.cards);
            int cardRnk = twoPair[3].rankIndex;
            int tmp = HandMapper::handMap.at("twoPair")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::TWO_PAIR);
                plyr->hand.type = HAND_TYPE::TWO_PAIR;
                for (size_t i = allCards.size() - 1; i != 0; --i) {
                    if (allCards[i].rankIndex != twoPair[0].rankIndex &&
                        allCards[i].rankIndex != twoPair[3].rankIndex) {
                        plyr->highCardRnk = allCards[i].rankIndex;
                    }
                }
                message = "Two Pair";
            }
        } else if (assessment::hasPair(plyr->hand.cards)) {
            int cardRnk = assessment::getPair(plyr->hand.cards)[0].rankIndex;
            int tmp = HandMapper::handMap.at("pair")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->hand.type = HAND_TYPE::PAIR;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::PAIR);
                // high card is highest card not part of the pair
                if (allCards[6].rankIndex != cardRnk) {
                    plyr->highCardRnk = allCards[6].rankIndex;
                } else {
                    plyr->highCardRnk = allCards[4].rankIndex;
                }
                message = "a Pair";
            }
        } else {
            std::sort(plyr->hand.cards.begin(), plyr->hand.cards.end());
            int cardRnk = plyr->hand.cards[4].rankIndex;
            int tmp = HandMapper::handMap.at("high")[cardRnk];
            if (tmp > plyr->hand.strength) {
                bestHand = plyr->hand.cards;
                plyr->hand.strength = tmp;
                plyr->hand.type = HAND_TYPE::HIGH_CARD;
                plyr->hand.hand_str = assessment::handTypeToString(HAND_TYPE::HIGH_CARD);
                message = "a High Card";
                plyr->highCardRnk = 0;
            }
        }
    }
    LOG << plyr->name << " has " << message << " with a hand strength of " <<
          plyr->hand.strength << " with a high card value of " << plyr->highCardRnk << END;
    plyr->hand.cards = bestHand;
    plyr->holeCards.first = hole1;
    plyr->holeCards.second = hole2;
}

void TexasHoldem::calculateHands() {
    for (auto &plyr : players) {
        findHand(&plyr);
    }
}

void TexasHoldem::findWinner() {
    /*
        1. check to see if every player has been assigned a handstrenght value
        2. sort players by their hand strengths low to high, using HandComparator functor ()
        3. check top players for a tie, if no tie determine winner
        4. if a tie is present must use high card rank to break tie
        5. winner collects pot
        6. if a tie then pot is split amongst people involved in tie.

    issues:
      - first sorting call changes order of players vector, thus changing the order player go in
        * possibly make a copy of the players first then call collectPot using players[i] pointer
    */
    LOG << END;
    // ensure all players have assessed there hand strengths
    for (auto &p : players) {
        if (p.hand.strength == 0)
            findHand(&p);
    }
    // put players in acesceding order iaw their handstrengths
    sort(players.begin(), players.end(), HandComparator()); //TODO: may change player order

    // check player order
    for (Player plyr : players)
        LOG << "handStrengths " << plyr.hand.strength << std::endl;

    // continue with find winner logic [. . . . . .]
    int cnt = 0;
    size_t maxInd = players.size() - 1;
    for (size_t i = 1; i <= maxInd; i++) {
        if (players[maxInd].hand.strength == players[maxInd - i].hand.strength)
            cnt++;
    }
    LOG << "Count of ties: " << cnt << std::endl;
    std::vector<Player> tiedPlayers;
    if (cnt > 0) {
        for (int i = 0; i <= cnt; i++)
            tiedPlayers.push_back(players[maxInd - i]);
    } else {
        LOG << players[maxInd].name << " has won the hand!!" << std::endl;
        players[maxInd].collectPot(pot);
        return;
    }

    // debug line
    LOG << "tiedPlayers.size -->  " << tiedPlayers.size() << END;

    // break tie if present using tiedPlayers vector
    sort(tiedPlayers.begin(), tiedPlayers.end(), HighCardRnkComparator());
    int stillTied = 0;

    // if we still have a tie then split pot amongst the winners
    if (tiedPlayers[tiedPlayers.size() - 2].highCardRnk ==
                                                tiedPlayers[tiedPlayers.size() - 1].highCardRnk) {
        for (auto p : tiedPlayers) {
            if (p.highCardRnk == tiedPlayers[tiedPlayers.size() - 1].highCardRnk)
                stillTied++;    // count of how many people are tied
        }

        // debug line
        LOG << "We have " << stillTied << " players " << "still tied." << std::endl;

        std::vector<std::string> winningNames;
        for (int i = 0; i < stillTied; ++i)    // [ * * * * *]  say stillTied = 3; loop goes 0 1 2
            winningNames.push_back(tiedPlayers[tiedPlayers.size() - 1 - i].name);

        size_t numTies = winningNames.size();
        size_t amtAwarded = pot / numTies;            // to split pot amongst ppl involved in the tie

        LOG << "Pot must be split amongst " << numTies << " players\n" << std::endl;
        for (auto &name : winningNames) {                // award winners split pot
            Player *winner = findPlayer(name);
            winner->collectPot((int) amtAwarded);
        }
    } else {
        // winner of hand when tie is broken
        std::string winnerName(tiedPlayers[tiedPlayers.size() - 1].name);
        Player *winner = findPlayer(winnerName);
        winner->collectPot(pot);
    }
}

Player *TexasHoldem::findPlayer(const std::string &name) {
    // private method that returns pointer to player identified by the name
    for (auto &player : players) {
        if (player.name == name) {
            return &player;
        }
    }
    return nullptr;
}

cardSuperVector TexasHoldem::permuteCards(const Player& plyr) const {
    LOG << "NEED IMPLEMENTATION..." << END;
    return {{}};
}

cardSuperVector TexasHoldem::comboCards(const Player *plyr) const {
    LOG << "generating possible 5 card hands..." << END;
    if (tableCards.size() != 5) return {{}};
    cardSuperVector result;
    Cards tmpHand;

    Cards plyrCards = tableCards;
    plyrCards.push_back(plyr->holeCards.first);
    plyrCards.push_back(plyr->holeCards.second);

    for (const auto& indexTmp : HAND_INDEX_COMBINATIONS) {
        for (const auto& ind : indexTmp)
            tmpHand.push_back(plyrCards[ind]);
        result.push_back(tmpHand);
        tmpHand.clear();
    }
    LOG << "result.size(): " << result.size() << END;
    return result;
}

void TexasHoldem::resetHand() {
    pot = 0;
    gameDeck.resetDeck();
    tableCards.clear();
    for (auto &player : players)
        player.reset();
    LOG << "======================hand reset!!=========================" << END;
}

intComb comb(uint64_t n, uint64_t k) {
    intComb result;
    std::vector<int> maskVect(k, 1);
    maskVect.resize((size_t) n, 0);
    std::vector<int> subVect;
    int cnt = 1;
    do {
        for (int i = 0; i < n; ++i) {
            if (maskVect[i]) {
                subVect.push_back(i);
            }
        }
        result.push_back(subVect);
        subVect.clear();
        cnt++;
    } while (prev_permutation(maskVect.begin(), maskVect.end()));
    return result;
}

uint64_t nCr(uint64_t n, uint64_t k) {
    uint64_t x = fact(n);
    uint64_t y = fact(k) * fact(n - k);
    return x / y;
}

uint64_t fact(uint64_t n) {
    uint64_t tmp = n - 1;
    while (tmp > 1) {
        n *= tmp;
        tmp -= 1;
    }
    return n;
}

void debugPrintVect(std::vector<int> vect) {
    LOG << "Current vector values: ";
    for (auto i: vect) {
        LOG << i << " ";
    }
    LOG << "\nEnd debug print\n\n" << std::endl;
}

void TexasHoldem::collectPot(Player *plyr) {
    plyr->collectPot(pot);
}

void TexasHoldem::gameStatus() const {
    LOG
            << "=========================Game Status================================================"
            << std::endl;
    LOG << "Small Blind: " << smallBlind << "\nBig Blind: " << bigBlind << std::endl;
    LOG << "Pot: " << pot << "\n" << std::endl;
    for (auto plyr : players) {
        showHand(&plyr);
    }
    showTableCards();
    LOG << "====================================================================================="
          << std::endl;

}

void TexasHoldem::gameOverview() const {
    LOG << "\n\n=========================Game Overview=========================================="
          << std::endl;
    LOG << "Game ID: " << gameId << std::endl;
    LOG << "Small Blind: " << smallBlind << std::endl;
    LOG << "Big Blind: " << bigBlind << "\n" << std::endl;
    std::ostringstream ss;
    for (auto &plyr : players) {
        ss << plyr << "\n";
    }
    LOG << ss.str();
    LOG
            << "===================================================================================="
            << std::endl;
}

void TexasHoldem::assign_dealer() {
    // function to assign dealer at beginning of game
    // helper function to be called in the ctor
    LOG << "NumPlayers: " << numPlayers << std::endl;
    std::random_device rd;
    uint32_t index_player = rd() % numPlayers;
    LOG << "Index of player: " << index_player << std::endl;
    getPlayerIterator(index_player)->dealer = true;
    dealerIndex = (uint32_t) index_player;
    if (index_player >= (numPlayers - 1)) {
        index_player = 0;
        dealerIndex = 0;
        getPlayerIterator(index_player)->smallBlind_bet = true;
        getPlayerIterator(++index_player)->bigBlind_bet = true;
    } else {
        getPlayerIterator(++index_player)->smallBlind_bet = true;
        dealerIndex = (uint32_t) index_player;      // next hands dealer
        if (index_player >= (numPlayers - 1)) {
            getPlayerIterator(0)->bigBlind_bet = true;
        } else {
            getPlayerIterator(index_player)->bigBlind_bet = true;
        }
    }
}

void TexasHoldem::rotateDealer() {
    // rotates to next dealer, small and big blind players
    for (auto &plyr : players) {
        plyr.dealer = false;
        plyr.smallBlind_bet = false;
        plyr.bigBlind_bet = false;
    }
    LOG << "Dealer Index: " << dealerIndex << std::endl;
    getPlayerIterator(dealerIndex++)->dealer = true;
    if (dealerIndex >= numPlayers - 1) {
        dealerIndex = 0;
        LOG << "smallBlind Index: " << dealerIndex << std::endl;
        getPlayerIterator(dealerIndex++)->smallBlind_bet = true;
        LOG << "bigBlind Index: " << dealerIndex << std::endl;
        getPlayerIterator(dealerIndex)->bigBlind_bet = true;
    } else {
        int tmp = dealerIndex;
        LOG << "smallBlind Index: " << dealerIndex << std::endl;
        getPlayerIterator(dealerIndex++)->smallBlind_bet = true;
        LOG << "smallBlind Index: " << dealerIndex << std::endl;
        getPlayerIterator(dealerIndex)->bigBlind_bet = true;
        dealerIndex = (uint32_t) tmp;
    }
}

std::ostream &operator<<(std::ostream& os, TexasHoldem& g1) {
    os << "Game Status: \n";
    os << "\tgame ID: " << g1.gameId << "\n";
    os << "\tsmallBlind: " << g1.smallBlind << "\n";
    os << "\tbigBlind: " << g1.bigBlind << "\n";

    os << "\nPlayers: \n";
    for (const auto& plr: g1.players)
        os << "\t" << plr << "\n";
    os << "\tdealer: " << g1.getPlayerIterator(g1.dealerIndex)->name << "\n";
    os << "game Stats: \tNEEDS IMPLEMENTATION!" << std::endl;
    return os;
}

TexasHoldem& TexasHoldem::operator=(TexasHoldem game) {
    this->smallBlind = game.smallBlind;
    this->bigBlind = game.bigBlind;
    this->gameDeck = game.gameDeck;
    this->players = game.players;
    this->gameId = game.gameId;
    this->dealerIndex = game.dealerIndex;
    this->numPlayers = game.numPlayers;
    this->pot = game.pot;
    this->tableCards = game.tableCards;
    return *this;
}

bool TexasHoldem::operator==(const TexasHoldem& game) const {
    if (this->gameId != game.gameId) return false;
    if (this->dealerIndex != game.dealerIndex) return false;
    if (this->players != game.players) return false;
    if (this->bigBlind != game.bigBlind) return false;
    if (this->smallBlind != game.smallBlind) return false;
    if (this->pot != game.pot) return false;
    if (this->tableCards != game.tableCards) return false;
    if (this->gameDeck != game.gameDeck) return false;
    if (this->numPlayers != game.numPlayers) return false;
    return true;
}

