#include "card.h"
#include <chrono>
#include <random>
#include <iostream>

const std::string Card::rank[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
									 "A" };      // index range:: 0 - 12
const std::string Card::suit[4] = { "C", "D", "H", "S" };

Card::Card()
{
	/*
		generates a random card
	*/
	static int randomizer;
	std::minstd_rand gen(
			std::chrono::system_clock::now().time_since_epoch().count() + randomizer );
	rankIndex = gen() % 13;
	suitIndex = gen() % 4;

	rk = Card::rank[ rankIndex ];
	st = Card::suit[ suitIndex ];
	// std::cout << "DEBUG:@CARD()::randomizer: " << randomizer << std::endl;
	randomizer += 454;
}

Card::Card( int rkInd, int stInd )
{
	if ( rkInd > 12 || rkInd < 0 || stInd > 3 || stInd < 0 )
	{
		std::cerr << "Error: rank/suit index out of range setting card to (0,0) \n";
		rankIndex = 0;
		suitIndex = 0;
		st = suit[ 0 ];
		rk = rank[ 0 ];
		std::cout << "testing " << std::endl;
	}
	else
	{
		rankIndex = rkInd;
		suitIndex = stInd;
		rk = rank[ rkInd ];
		st = suit[ stInd ];
	}
}

Card::~Card()
{};


const std::string Card::getRank() const
{
	return rk;
}


const std::string Card::getSuit() const
{
	return st;
}


std::ostream& operator<<( std::ostream& os, const Card& cd )
{
	os << cd.rk << " of " << cd.st << "\n";
	return os;
}


int Card::cmp( const Card& cd ) const
{
	int result;
	if ( rankIndex == cd.rankIndex && suitIndex == cd.suitIndex )
	{
		result = 0;
	}
	else
	{
		result = ( rankIndex < cd.rankIndex ) ? -1 : 1;
	}
	return result;
}


bool Card::operator==( const Card& cd ) const
{
	return !cmp( cd );
}


bool Card::operator<( const Card& cd ) const
{
	return cmp( cd ) < 0;
}


bool Card::operator>( const Card& cd ) const
{
	return cmp( cd ) > 0;
}


bool CardCmp::operator()( Card cd1, Card cd2 ) const
{
	// note to a set two objects are equal iif !(a < b) && !(b < a) is true
	int rkIndex1 = cd1.rankIndex;
	int stIndex1 = cd1.suitIndex;
	int rkIndex2 = cd2.rankIndex;
	int stIndex2 = cd2.suitIndex;
	if ( rkIndex1 == rkIndex2 && stIndex1 != stIndex2 )
	{
		return stIndex1 < stIndex2;
	}
	else if ( rkIndex1 != rkIndex2 && stIndex1 == stIndex2 )
	{
		return rkIndex1 < rkIndex2;
	}
	else
	{
		return cd1 < cd2;
	}
}
