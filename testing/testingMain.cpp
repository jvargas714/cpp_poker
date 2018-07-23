#include <iostream>
#include <memory>
#include "gtest/gtest.h"
#include "master_poker.h"

// globals
const std::string TEST_CFG_PATH = "../config/test1.cfg";
std::unique_ptr<TexasHoldem> g_game;

TEST (ctorTests, defaultCtor) {
    TexasHoldem game;
    LOG << "\n" << game << END;
    ASSERT_TRUE(game.players.size()==2);
}

TEST (ctorTests, copyCtor) {
    TexasHoldem game(*g_game);
    LOG << "\n" << game << END;
    ASSERT_EQ(game, *g_game);
}

TEST (ctorTests, initListNames) {
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
    auto plyr1 = game.getPlayerIterator("mark");
    auto plyr2 = game.getPlayerIterator("scott");
    auto plyr3 = game.getPlayerIterator("john");
    ASSERT_TRUE(plyr1->cash == 1234);
    ASSERT_TRUE(plyr2->cash == 5);
    ASSERT_TRUE(plyr3->cash == 1500);
}

TEST (ctorTests, cfgLoad) {
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
    ASSERT_TRUE(hnd.strength == 37);
}

TEST (assessTest, trips) {
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
    ASSERT_TRUE(hnd.strength == 51);
}

TEST (assessTest, str8) {
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
    ASSERT_TRUE(hnd.strength == 61);
}

TEST (assessTest, flush) {
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
    ASSERT_TRUE(hnd.strength == 75);
}

TEST (assessTest, fullHouse) {
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
    ASSERT_TRUE(hnd.strength == 86);
}

TEST (assessTest, fourOfaKind) {
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
    ASSERT_TRUE(hnd.strength == 92);
}

TEST (assessTest, str8Flush) {
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
    ASSERT_TRUE(hnd.strength == 111);
}

TEST (assessTest, royalFlush) {
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

void setupGlobalGame() {
    g_game = std::make_unique<TexasHoldem>( );
    g_game->enterGame("Player 3", 1500);
    g_game->enterGame("Player 4", 1500);
    g_game->enterGame("Player 5", 2000);
    g_game->enterGame("Player 6", 2500);
}




// --------------------------------start tests----------------------------------
int main(int argc, char** argv) {
    setupGlobalGame();
    std::cout << "Google Testing Poker....." << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
