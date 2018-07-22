#include <iostream>
#include "gtest/gtest.h"
#include "master_poker.h"

// globals
const std::string TEST_CFG_PATH = "../config/test1.cfg";
TexasHoldem* g_game;

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

bool setupGlobalGame() {
    g_game = new TexasHoldem();
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

