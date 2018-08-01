#include <iostream>
#include "gtest/gtest.h"
#include "master_poker.h"

// globals
const std::string TEST_CFG_PATH = "../config/test1.cfg";
TexasHoldem* g_game;

static void clearCash() {
    for (auto &plyr : g_game->players)
        plyr.cash = 0;
}

TEST (ctorTests, defaultCtor) {
    TexasHoldem game;
    LOG_TRACE << "\ntesting game ctor: \n" << game << END;
    ASSERT_TRUE(game.players.size()==2);
}

TEST (ctorTests, copyCtor) {
    TexasHoldem game(*g_game);
    LOG_TRACE << "\n" << game << END;
    ASSERT_EQ(game, *g_game);
}

TEST (ctorTests, initListNames) {
    LOG_TRACE << "ctorTests --> initListNames" << END;
    std::initializer_list<std::string> lst = {"jake", "john", "jerry", "jacob", "silvia"};
    TexasHoldem game(lst);
    ASSERT_TRUE(game.numPlayers == 5);
    auto plyr1 = game.getPlayerIterator("jake");
    auto plyr2 = game.getPlayerIterator("john");
    auto plyr3 = game.getPlayerIterator("jerry");
    auto plyr4 = game.getPlayerIterator("jacob");
    auto plyr5 = game.getPlayerIterator("silvia");
    ASSERT_TRUE(plyr1->name == "jake");
    ASSERT_TRUE(plyr2->name == "john");
    ASSERT_TRUE(plyr3->name == "jerry");
    ASSERT_TRUE(plyr4->name == "jacob");
    ASSERT_TRUE(plyr5->name == "silvia");
}

TEST (ctorTests, initListNameMoney) {
    std::initializer_list<std::pair<std::string, int>> lst = {{"mark", 1234}, {"scott", 5}, {"john", 1500}};
    TexasHoldem game(lst);
    LOG_TRACE << "ctorTests :: initListNameMoney -->" << END;
    auto plyr1 = game.getPlayerIterator("mark");
    auto plyr2 = game.getPlayerIterator("scott");
    auto plyr3 = game.getPlayerIterator("john");
    ASSERT_TRUE(plyr1->cash == 1234);
    ASSERT_TRUE(plyr2->cash == 5);
    ASSERT_TRUE(plyr3->cash == 1500);
}

TEST (ctorTests, cfgLoad) {
    LOG_TRACE << "ctorTests :: cfgLoad :: " << END;
    TexasHoldem game(TEST_CFG_PATH);
    ASSERT_TRUE(game.getGameId() == 7140);
    ASSERT_TRUE(game.getBigBlind() == 50);
    ASSERT_TRUE(game.getSmallBlind() == 25);
    ASSERT_TRUE(game.dealerIndex == 2);
    ASSERT_TRUE(game.players[0].name == "Jay");
    ASSERT_TRUE(game.players[2].cash == 1500 && game.players[2].name == "Oscar");
    ASSERT_TRUE(game.numPlayers == 6);
}

// [0  1  2  3  4  5  6  7  8   9  10 11 12]
// [2  3  4  5  6  7  8  9  10  j  q  k  a]
TEST (assessTest, highCard) {
    LOG_TRACE << "assessTest :: highCard: " << END;
    Cards cards = {
            Card(1, 0),  // 3
            Card(2, 0),  // 4
            Card(8, 2),
            Card(9, 3),
            Card(10, 1)  // Q
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(hnd.strength == 12);
}

TEST (assessTest, pairs) {
    LOG_TRACE << "assessTest :: pairs: " << END;
    // pair of 6
    Cards cards = {
            Card(4, 1),
            Card(7, 1),
            Card(6, 0),
            Card(4, 0),
            Card(5, 0)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(hnd.strength == 19);
}

TEST (assessTest, twoPairs) {
    LOG_TRACE << "assessTest :: twoPairs: " << END;
    // pair of 6 and k
    Cards cards = {
            Card(4, 1),
            Card(11, 1),
            Card(6, 0),
            Card(4, 0),
            Card(11, 0)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(hnd.strength == 39);
}

TEST (assessTest, trips) {
    LOG_TRACE << "assessTest :: trips: " << END;
    // trips Ace
    Cards cards = {
            Card(12, 1),
            Card(3, 1),
            Card(12, 0),
            Card(4, 3),
            Card(12, 2)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::TRIPS);
    ASSERT_TRUE(hnd.strength == 53);
}

TEST (assessTest, str8) {
    LOG_TRACE << "assessTest :: str8: " << END;
    // straight
    Cards cards = {
            Card(5, 1),
            Card(6, 1),
            Card(7, 0),
            Card(8, 3),
            Card(9, 2)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::STRAIGHT);
    ASSERT_TRUE(hnd.strength == 63);
}

TEST (assessTest, flush) {
    LOG_TRACE << "assessTest :: flush: " << END;
    // 9 high straight flush
    Cards cards = {
            Card(0, 0),
            Card(4, 0),
            Card(1, 0),
            Card(6, 0),
            Card(10, 0)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::FLUSH);
    ASSERT_TRUE(hnd.strength == 77);
}

TEST (assessTest, fullHouse) {
    LOG_TRACE << "assessTest :: fullHouse: " << END;
    // 9 high straight flush
    Cards cards = {
            Card(8, 2),
            Card(1, 0),
            Card(8, 2),
            Card(1, 1),
            Card(8, 3)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::FULL_HOUSE);
    ASSERT_TRUE(hnd.strength == 88);
}

TEST (assessTest, fourOfaKind) {
    LOG_TRACE << "assessTest :: fourOfaKind: " << END;
    // four 3's
    Cards cards = {
            Card(1, 0),
            Card(4, 1),
            Card(1, 3),
            Card(1, 2),
            Card(1, 1)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::FOUR_OF_A_KIND);
    ASSERT_TRUE(hnd.strength == 94);
}

TEST (assessTest, str8Flush) {
    LOG_TRACE << "assessTest :: str8Flush: " << END;
    // 9 high straight flush
    Cards cards = {
            Card(5, 2),
            Card(4, 2),
            Card(7, 2),
            Card(6, 2),
            Card(3, 2)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::STRAIGHT_FLUSH);
    ASSERT_TRUE(hnd.strength == 113);
}

TEST (assessTest, royalFlush) {
    LOG_TRACE << "assessTest :: royalFlush: " << END;
    // 9 high straight flush
    Cards cards = {
            Card(10, 3),
            Card(11, 3),
            Card(12, 3),
            Card(9, 3),
            Card(8, 3)
    };
    Hand hnd = assessment::findHandStrength(cards);
    ASSERT_TRUE(hnd.type == HAND_TYPE::ROYAL_FLUSH);
    ASSERT_TRUE(hnd.strength == 150);
}

TEST (inGameAssess, inGameHighCardDueToTie) {
    LOG_TRACE << "inGameAssess :: inGameHighCardDueToTie: " << END;
    // set winnings
    g_game->setPot(250);

    // PAIR High card King
    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(TWO, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(THREE, CLUB);

    // PAIR High card King
    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    // PAIR High card King
    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(EIGHT, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(NINE, HEART);

    // PAIR high card king
    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(THREE, SPADE);

    // // PAIR high card king
    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    // PAIR high card ace (winner)
    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(SEVEN, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(QUEEN, SPADE));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(QUEEN, DIAMOND));
    g_game->getTableCards().push_back(Card(TEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::PAIR);

    // player 6 should win via high card as everyone else has a pair from table due to a tie
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, pairWin) {
    LOG_TRACE << "inGameAssess::pairWIn: >> " << END;
    // set winnings
    g_game->setPot(250);

    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(TWO, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(THREE, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    // winner Queen pair
    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(EIGHT, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(QUEEN, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(QUEEN, DIAMOND));
    g_game->getTableCards().push_back(Card(TEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::HIGH_CARD);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, tripsWin) {
    LOG_TRACE << "inGameAssess::tripsWin >> " << END;
    // set winnings
    g_game->setPot(250);

    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(TWO, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(THREE, CLUB);

    // three of a kind winner
    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(TEN, DIAMOND);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TEN, CLUB);

    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(EIGHT, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(QUEEN, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(QUEEN, DIAMOND));
    g_game->getTableCards().push_back(Card(TEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::TRIPS);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::HIGH_CARD);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, fourOfaKindWin) {
    LOG_TRACE << "inGameAssess::fourOfaKind >>" << END;
    // set winnings
    g_game->setPot(250);

    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(TWO, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(THREE, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(EIGHT, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(QUEEN, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    // four of a kind
    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(SEVEN, DIAMOND);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(SEVEN, HEART);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(SEVEN, CLUB));
    g_game->getTableCards().push_back(Card(QUEEN, DIAMOND));
    g_game->getTableCards().push_back(Card(SEVEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::FOUR_OF_A_KIND);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, fullHouseHighTrips) {
    LOG_TRACE << "inGameAssess::fullHouseHighTrips >> " << END;
    // set winnings
    g_game->setPot(250);

    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(QUEEN, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(THREE, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    // winner 3 KINGs  2 QUEENs
    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(KING, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(KING, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(QUEEN, DIAMOND));
    g_game->getTableCards().push_back(Card(QUEEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::FULL_HOUSE);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::FULL_HOUSE);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::TWO_PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::PAIR);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, tiedPairWinKicker) {
    LOG_TRACE << "inGameAssess::tiedPairSplitPot >> " << END;

    // set winnings
    g_game->setPot(250);
    // pair of queens ace kicker
    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(QUEEN, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(ACE, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    // pair of queens king kicker
    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(QUEEN, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(TEN, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(JACK, DIAMOND));
    g_game->getTableCards().push_back(Card(QUEEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::HIGH_CARD);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->cash == 250);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, tiedPairSplitPot) {
    LOG_TRACE << "inGameAssess::tiedPairSplitPot >> " << END;

    // set winnings
    g_game->setPot(250);
    // pair of queens ACE kicker
    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(QUEEN, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(ACE, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(TWO, CLUB);

    // pair of queens ACE kicker
    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(QUEEN, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(ACE, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(FIVE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(THREE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, SPADE);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(JACK, DIAMOND));
    g_game->getTableCards().push_back(Card(QUEEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::PAIR);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::HIGH_CARD);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->cash == 125);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->cash == 125);

    clearCash();

    // reset table
    g_game->resetHand();
}

TEST (inGameAssess, highCardWins) {
    LOG_TRACE << "inGameAssess::tiedPairSplitPot >> " << END;

    // set winnings
    g_game->setPot(250);
    // winner high card of ACE
    g_game->getPlayerIterator("Player 1")->holeCards.first = Card(TEN, SPADE);
    g_game->getPlayerIterator("Player 1")->holeCards.second = Card(SIX, CLUB);

    g_game->getPlayerIterator("Player 2")->holeCards.first = Card(FOUR, SPADE);
    g_game->getPlayerIterator("Player 2")->holeCards.second = Card(ACE, CLUB);

    g_game->getPlayerIterator("Player 3")->holeCards.first = Card(TEN, DIAMOND);
    g_game->getPlayerIterator("Player 3")->holeCards.second = Card(SEVEN, HEART);

    g_game->getPlayerIterator("Player 4")->holeCards.first = Card(TWO, DIAMOND);
    g_game->getPlayerIterator("Player 4")->holeCards.second = Card(ACE, SPADE);

    g_game->getPlayerIterator("Player 5")->holeCards.first = Card(TWO, HEART);
    g_game->getPlayerIterator("Player 5")->holeCards.second = Card(ACE, DIAMOND);

    g_game->getPlayerIterator("Player 6")->holeCards.first = Card(NINE, SPADE);
    g_game->getPlayerIterator("Player 6")->holeCards.second = Card(ACE, HEART);

    // set table
    g_game->getTableCards().push_back(Card(THREE, HEART));
    g_game->getTableCards().push_back(Card(KING, CLUB));
    g_game->getTableCards().push_back(Card(FIVE, CLUB));
    g_game->getTableCards().push_back(Card(JACK, DIAMOND));
    g_game->getTableCards().push_back(Card(QUEEN, SPADE));

    // assess table
    g_game->findWinner();

    // check status of players
    ASSERT_TRUE(g_game->getPlayerIterator("Player 1")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 3")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->hand.type == HAND_TYPE::HIGH_CARD);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->hand.type == HAND_TYPE::HIGH_CARD);

    // player 3 wins via queen pair
    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->highCardRnk == 12);

    ASSERT_TRUE(g_game->getPlayerIterator("Player 4")->cash == 62);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 6")->cash == 62);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 2")->cash == 62);
    ASSERT_TRUE(g_game->getPlayerIterator("Player 5")->cash == 62);

    clearCash();

    // reset table
    g_game->resetHand();
}

void setupGlobalGame() {
    g_game = new TexasHoldem();
    g_game->enterGame("Player 3", 0);
    g_game->enterGame("Player 4", 0);
    g_game->enterGame("Player 5", 0);
    g_game->enterGame("Player 6", 0);
    g_game->getPlayerIterator("Player 1")->cash = 0;
    g_game->getPlayerIterator("Player 2")->cash = 0;
}

// --------------------------------start tests----------------------------------
int main(int argc, char** argv) {
    setupGlobalGame();
    std::cout << "Google Testing Poker....." << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

