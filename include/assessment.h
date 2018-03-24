/*
 * assessment.h
 *  Created on: Aug 9, 2016
 *      Author: jay
 *  Description:
 *  	- functions to assess table
 */

#ifndef HEADERS_ASSESSMENT_H_
#define HEADERS_ASSESSMENT_H_
#include "card.h"
#include "deck.h"
#include "poker_types.h"
#include "poker_error.h"

typedef enum
{
	NONE,
	HIGH_CARD,
	PAIR,
	TWO_PAIR,
	TRIPS,
	FOUR_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	STRAIGHT_FLUSH,
	ROYAL_FLUSH
} HAND_TYPE;

typedef struct Hand
{
	HAND_TYPE 	type;
	Cards 		cards;
	int 		strength;
	std::string hand_str;
	Hand(): type(NONE), strength(0), hand_str("") {}
} HAND;

namespace assessment
{
	std::vector<std::string> getHandSuits( const Cards& );

	int* getRankInd( const Cards& );

	std::vector<int> dupRankInd( const Cards& );

	// has functions 
	bool hasPair( const Cards& );

	bool hasTwoPair( const Cards& );

	bool hasTrips( const Cards& );

	bool hasFourOfAKind( const Cards& );

	bool hasStraight( const Cards& );

	bool hasFlush( const Cards& );

	bool hasFullHouse( const Cards& );

	bool hasStraightFlush( const Cards& );

	bool hasRoyalFlush( const Cards& );

	HAND findHandStrength( const Cards& );

	// get functions
	Cards getPair( const Cards& );

	Cards getTwoPair( const Cards& );

	Cards getTrips( const Cards& );

	Cards getFourOfAKind( const Cards& );

	Cards getStraight( const Cards& );

	Cards getFlush( const Cards& );

	Cards getFullHouse( const Cards& );

	Cards getStraightFlush( const Cards& );

	Cards getRoyalFlush( const Cards& );

} // END assessment namespace
#endif /* HEADERS_ASSESSMENT_H_ */
