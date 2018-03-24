#include "gtest/gtest.h"
#include "master_poker.h"

class TexasHoldemTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    TexasHoldem game;
};
