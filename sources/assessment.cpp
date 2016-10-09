/*
 * assessment.cpp
 *
 *  Created on: Aug 9, 2016
 *      Author: jay
 */
#include "assessment.h"

vector<string> assessment::getHandSuits( const Cards& cds )
{
    vector<string> suits;
    for( Card cd: cds )
    {
        suits.push_back( cd.getSuit() );
    }
    return suits;
}

int* assessment::getRankInd( const Cards& cds )
{
    int rankInds[5];
    int n = 0;
    std::for_each(cds.begin(), cds.end(), [&n, &rankInds](Card cd)
    {
        rankInds[n] = cd.rankIndex;
        n++;
    });
    return rankInds;
}

vector<int> assessment::dupRankInd( const Cards& cds )
{
    using std::begin;
    using std::end;

    int *handRanks = assessment::getRankInd( cds );
    vector<int> dupRanksInd;
    int cnt = 0;
    int tmp = 0;
    int index = 0;

    for(int i = 0; i<13; i++)
    {
        for(int *rk = handRanks; rk < (handRanks+5); rk++)
        {
            if(*rk == i)
            {
                cnt++;
                if(cnt == 1)
                {
                    tmp = index;
                }
                else if(cnt == 2)
                {
                    dupRanksInd.push_back(tmp);
                    dupRanksInd.push_back(index);
                }
                else if(cnt > 2)
                {
                    dupRanksInd.push_back(index);
                }
            }index++;
        }
        index = 0;
        cnt = 0;
        tmp = 0;
    }
    return dupRanksInd;
}

bool assessment::hasPair( const Cards& cds )
{
    vector<int> dupRanks = assessment::dupRankInd( cds );
    bool condition = false;
    if(dupRanks.size() == 2)
    {
        condition = true;
    }
    return condition;
}

bool assessment::hasTwoPair( const Cards& cds )
{
    vector<int> handInd = assessment::dupRankInd( cds );
    int* handRanks = assessment::getRankInd( cds );

    // checks to see if we have 4 in handInd and its not a 4 of a kind
    bool condition = handInd.size() == 4 &&
                     std::count(handRanks, handRanks+5,
                                cds[handInd[0]].rankIndex) != 4;
return condition;
}

bool assessment::hasTrips( const Cards& cds )
{
    vector<int> dupInd = assessment::dupRankInd( cds );
    return dupInd.size() == 3;
}

bool assessment::hasFourOfAKind( const Cards& cds )
{
    using std::count;
    vector<int> dupInd = assessment::dupRankInd( cds );
    if(dupInd.size() == 4)
    {
        Card sampleCard = cds[dupInd[0]];
        return count(cds.begin(), cds.end(), sampleCard) == 4;
    }
    else
        return false;
}

bool assessment::hasStraight( const Cards& cds )
{
    vector<string> suits = assessment::getHandSuits( cds );
    std::sort(cds.begin(), cds.end());
    int cnt = 0;
    for(int i = 1; i < 5; i++)
    {
        if(cds[i].rankIndex - cds[i-1].rankIndex == 1)
            cnt++;
    }
    return cnt == 4 && count(suits.begin(), suits.end(), suits[0]) != 5;
}

bool assessment::hasFlush( const Cards& cds )
{
    using std::count;

    vector<string> suits = assessment::getHandSuits( cds );
    Cards cards = cds;
    std::sort(cards.begin(), cards.end());
    int cnt = 0;

    for( int i = 1; i < 5; i++ ){
        if( cards[i].rankIndex - cards[i-1].rankIndex == 1 )
            cnt++;
    }
    // checking for a flush only and not a straight flush
    return count( suits.begin(), suits.end(), suits[0] ) == 5 && cnt != 4
						&& !assessment::hasFullHouse( cds );
}

bool assessment::hasFullHouse( const Cards& cds )
{
    using std::count;
    Cards cards = cds;

    if( cards.size() != 5 )
        throw poker_error::HandIDError("Error: @hasFullHouse -->hand size not 5 cards");

    sort( cards.begin(), cards.end() );
    int a = count( cards.begin(), cards.end(), cards[0] );        // either 2, 3, or 0
    int b = count( cards.begin(), cards.end(), cards[4] );        // either 2, 3, or 0
    return (a+b) == 5;      // if a pair and trips in one hand then we have a fullHouse
}

bool assessment::hasStraightFlush( const Cards& cds )
{
    Cards cards = cds;
    vector<string> suits = assessment::getHandSuits( cds );
    std::sort( cards.begin(), cards.begin() );
    int rnkCnt = 0, stCnt = 0;

    for( int i = 1; i < 5; i++ )
    {
        if(cards[i].rankIndex - cards[i-1].rankIndex == 1)
            rnkCnt++;
    }
    stCnt = std::count( suits.begin(), suits.end(), suits[0] );
    return stCnt == 5 && rnkCnt == 4 && cards[4].getRank() != "A";
}

bool assessment::hasRoyalFlush( const Cards& cds )
{
    Cards cards = cds;
    vector<string> suits = assessment::getHandSuits( cds );
    sort( cards.begin(), cards.end() );
    int rnkCnt = 0, stCnt = 0;

    for( int i = 1; i < 5; i++ )
    {
        if( cards[i].rankIndex - cards[i-1].rankIndex == 1 )
            rnkCnt++;
    }
    stCnt = std::count( suits.begin(), suits.end(), suits[0] );
    return stCnt == 5 && rnkCnt == 4 && cards[4].getRank() == "A";
}

Cards assessment::getPair( const Cards& cds )
{
    Cards pair;
    if( !hasPair( cds ) )
        throw poker_error::HandIDError("Error--> assessment::getPair(): HandIDError");
    else
    {
        vector<int> rankIndex = dupRankInd( cds );
        pair.push_back( cds[ rankIndex[0] ] );
        pair.push_back( cds[ rankIndex[1] ] );
    }
    return pair;
}

Cards assessment::getTwoPair( const Cards& cds )
{
    Cards cards;
    if( !hasTwoPair( cds ) )
        throw (poker_error::HandIDError( "HandIDError-->assessment::getTwoPair()") );

    else
    {
        vector<int> dupInd = dupRankInd( cds );
        auto i = dupInd.begin();

        while( i != dupInd.end() )
        {
            cards.push_back( cds[*i] );
            i++;
        }
    }
    sort( cards.begin(), cards.end() );
    return cards;
}

Cards assessment::getTrips( const Cards& cds )
{
    Cards cards;
    if( !hasTrips( cds ) )
        throw poker_error::HandIDError( "HandIDError-->assessment::getTrips()" );

    else 
    {
        vector<int> dupInd = dupRankInd( cds );
        auto i = dupInd.begin();

        while( i != dupInd.end() )
        {
            cards.push_back( cds[*i] );
            i++;
        }
    }
    return cards;
}

Cards assessment::getFourOfAKind(const Cards& cds )
{
    vector<int> dupInd = dupRankInd( cds );
    Cards cards;

    if( hasFourOfAKind( cds ) )
        throw poker_error::HandIDError("HandIDError-->assessment::getFourOfAKind()");

    else
    {
        auto i = dupInd.begin();
        while ( i != dupInd.end() )
        {
            cards.push_back( cds[*i] );
            i++;
        }
    }
    return cards;
}

Cards assessment::getStraight( const Cards& cds )
{
    if(!hasStraight(cds))
        throw poker_error::HandIDError("HandIDError-->assessment::getStraight()");

    else 
    {
        // copy bc cannot change hand in player object do to constness 
        Cards local_hand( cds );
        sort( local_hand.begin(), local_hand.end() );
        return local_hand;
    }
}

Cards assessment::getFlush( const Cards& cds )
{
    if( !hasFlush( cds ) )
        throw poker_error::HandIDError("HandIDError-->assessment::getFlush()");

    else
    {
        // copy due to constness 
        Cards local_hand( cds );
        sort( local_hand.begin(), local_hand.end() );
        return local_hand;
    }
}

Cards assessment::getFullHouse( const Cards& cds )
{
    if( !hasFullHouse( cds ) )
        throw poker_error::HandIDError("Error: HandIDError --> "
                    "@assessment::getFullHouse(), player does not have a full house!");
    else{
        // copy due to constness 
        Cards local_hand( cds );
        sort( local_hand.begin(), local_hand.end() );
        return local_hand;
    }
}

Cards assessment::getStraightFlush( const Cards& cds )
{
    if( !hasStraightFlush( cds ) )
        throw poker_error::HandIDError("Error: @assessment::getStraightFlush()-->HandIDError--> " 
            "player does not have a Straight Flush");
    else
        return cds;
}

Cards assessment::getRoyalFlush( const Cards& cds )
{
    if( !hasRoyalFlush( cds ) )
        throw poker_error::HandIDError("HandIDError: @assessment::getRoyalFlush -->" 
            " Player does not have royal flush");
    else
        return cds;
}

int assessment::findHandStrength( const Cards& cds )
{
    // runs through all assessment functions to determine a hand strength and return it
    using std::count;
    sort( cds.begin(), cds.end() );
    std::string message("error");
    int handStrength;

    if( hasRoyalFlush( cds ) )
    {
        handStrength    = HandMapper::handMap.at( "royalFlush" )[0];
        message         = "a Royal Flush";
    }
    else if( hasStraightFlush( cds ) )
    {
        int highCardRank = cds[4].rankIndex;
        handStrength     = HandMapper::handMap.at( "straightFlush" )[highCardRank];
        message          = "a Straight Flush";
    }
    else if( hasFourOfAKind( cds ) )
    {
        int cardRnk     = getFourOfAKind( cds )[0].rankIndex;
        handStrength    = HandMapper::handMap.at( "fourOfAKind" )[cardRnk];
        message         = "a Four of a Kind";
    }
    else if( hasFullHouse( cds ) )
    {
        Cards fullHouse = getFullHouse( cds );
        int cardRnk;

        if( count( fullHouse.begin(), fullHouse.end(), fullHouse[0] ) == 3 )
            cardRnk     = fullHouse[0].rankIndex;
        else
            cardRnk     = fullHouse[4].rankIndex;

        handStrength    = HandMapper::handMap.at( "fullHouse" )[ cardRnk ];
        message         = "a Full House";
    }
    else if( hasFlush( cds ) )
    {
        Cards flush     = getFlush( cds );
        int cardRnk     = flush[4].rankIndex;
        handStrength    = HandMapper::handMap.at( "flush" )[ cardRnk ];
        message         = "a Flush";
    }
    else if( hasStraight( cds ) )
    {
        int cardRnk     = getStraight( cds )[4].rankIndex;
        handStrength    = HandMapper::handMap.at( "straight" )[ cardRnk ];
        message         = "a Straight";
    }
    else if( hasTrips( cds ) )
    {
        int cardRnk     = getTrips( cds )[0].rankIndex;
        handStrength    = HandMapper::handMap.at( "trips" )[cardRnk];
        message         = "a Three of a Kind";
    }
    else if( hasTwoPair( cds ) )
    {
        Cards twoPair   = getTwoPair( cds );
        int cardRnk     = twoPair[3].rankIndex;
        handStrength    = HandMapper::handMap.at( "twoPair" )[cardRnk];
        message = "Two Pair";
    }
    else if( hasPair( cds ) )
    {
        int cardRnk     = getPair( cds )[0].rankIndex;
        handStrength    = HandMapper::handMap.at( "pair" )[cardRnk];
        message         = "a Pair";
    }
    else
    {
        sort( cds.begin(), cds.end() );
        int cardRnk     = cds[4].rankIndex;
        handStrength    = HandMapper::handMap.at( "high" )[cardRnk];
        message         = "a High Card";
    }
    std::cout << "assessment::findHandStrength(): " << "hand type: " << message <<
        ", with a hand strength of " << handStrength << std::endl;
    
    return handStrength;
}


