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
#include "logger.h"

Poker::Poker()
// creates new game defaulting to 0 players with default cash $1500
		: players( { Player( "Player 1", 1500 ), Player( "Player 2", 1500 ) } ), numPlayers( 2 ), gameDeck()
{}

Poker::Poker( int cash )
// creates new game defaulting to 2 players with specified starting cash
		: players( { Player( "Player 1", cash ), Player( "Player 2", cash ) } ), gameDeck()
{
	numPlayers = 2;
}

Poker::Poker( std::initializer_list<std::string> names )
/*
 * Default cash: $1500
 */
		: players(), gameDeck()
{
	for ( const std::string& name : names )
	{
	log() << __FUNCTION__ << " name " << name << "\n";
		players.emplace_back( Player( name, 1500 ) );
	}
	numPlayers = static_cast<uint32_t>(players.size());
}

Poker::Poker( std::initializer_list<std::pair<std::string, int> > list )
		: players(), gameDeck()
{
	/*
	 * Specify list of pairs player : cash
	 */
	using std::pair;
	for ( pair<std::string, int> nameCashPair : list )
	{
		players.push_back( Player( nameCashPair.first, nameCashPair.second ) );
	}
	numPlayers = list.size();
}

Poker::~Poker()
{};

/*
void Poker::enterGame(std::string &&name, int &&cash) {

    players.push_back(Player(std::move(name), std::move(cash)));
    log() << name << " has entered the game with $" << cash << "\n";
    numPlayers++;
    log() << "Number of players: " << numPlayers << "\n";
}*/

void Poker::enterGame( std::string name, int cash )
{
	players.push_back( Player( name, cash ) );
	log() << name << " has entered the game with $" << cash << "\n";
	numPlayers++;
	log() << "Number of players: " << numPlayers << "\n";
}

void Poker::enterGame( std::initializer_list<std::pair<std::string, int> > players )
{
	using std::pair;
	using std::move;

	for ( pair<std::string, int> player : players )
	{
		this->players.push_back( Player( player.first, player.second ) );
	}
}

void Poker::dealCard( Player* plyr, Card&& card )
{
	plyr->hand.push_back( std::move( card ) );
	log() << plyr->name << " has been dealt " << card << "\n";
}

void Poker::dealCard( Player* plyr )
{
	plyr->hand.push_back( Card() );
}

std::vector<int> Poker::dupRankInd( const Player* plyr ) const
{
	using std::begin;
	using std::end;

	int* handRanks = getRankInd( plyr );         // array pointing to first element in handRanks
	std::vector<int> dupRanksInd;
	int cnt = 0;
	int tmp = 0;
	int index = 0;

	for ( int i = 0; i < 13; i++ )
	{
		for ( int* rk = handRanks; rk < ( handRanks + 5 ); rk++ )
		{
			if ( *rk == i )
			{
				cnt++;
				if ( cnt == 1 )
				{
					tmp = index;
				}
				else if ( cnt == 2 )
				{
					dupRanksInd.push_back( tmp );
					dupRanksInd.push_back( index );
				}
				else if ( cnt > 2 )
				{
					dupRanksInd.push_back( index );
				}
			}
			index++;
		}
		index = 0;
		cnt = 0;
		tmp = 0;
	}
	delete[] handRanks;
	return dupRanksInd;
}

int* Poker::getRankInd( const Player* plyr ) const
{
	/*returns a pointer to an array containing rank indexes of each card
	 * Typically array is 5 elements*/
	int* rankInds;
	rankInds = new int[5];                       // will need to be deleted when not used
	int n = 0;
	std::for_each( plyr->hand.begin(), plyr->hand.end(), [ &n, &rankInds ]( Card cd )
	{
		rankInds[ n ] = cd.rankIndex;
		n++;
	} );
	return rankInds;
}

bool Poker::hasPair( const Player* plyr ) const
{
	/*returns true if player has a pair as best hand
		throws error in case that player doesn't have 5 cards in hand
	*/
	std::vector<int> dupRanks = dupRankInd( plyr );
	bool condition = false;
	if ( dupRanks.size() == 2 )
	{
		condition = true;
	}
	return condition;
}

Cards Poker::getPair( const Player* player ) const
{
	std::vector<Card> pair;
	if ( !hasPair( player ) )
	{
		throw poker_error::HandIDError( "Error--> Poker:getPair(): HandIDError" );
	}
	else
	{
		std::vector<int> rankIndex = dupRankInd( player );
		pair.push_back( player->hand[ rankIndex[ 0 ] ] );
		pair.push_back( player->hand[ rankIndex[ 1 ] ] );
	}
	return pair;
}

bool Poker::hasTwoPair( const Player* player ) const
{
	std::vector<int> handInd = dupRankInd( player );
	int* handRanks = getRankInd( player );
	// checks to see if we have 4 in handInd and its not a 4 of a kind
	bool condition = handInd.size() == 4 &&
					 std::count( handRanks, handRanks + 5,
								 player->hand[ handInd[ 0 ] ].rankIndex ) != 4;
	delete[] handRanks;
	return condition;
}

Cards Poker::getTwoPair( const Player* player ) const
{
	std::vector<Card> cards;

	if ( !hasTwoPair( player ) )
	{
		throw ( poker_error::HandIDError( "HandIDError-->getTwoPair()" ) );
	}

	else
	{
		std::vector<int> dupInd = dupRankInd( player );
		auto i = dupInd.begin();

		while ( i != dupInd.end() )
		{
			cards.push_back( player->hand[ *i ] );
			i++;
		}
	}
	sort( cards.begin(), cards.end() );
	return cards;
}

bool Poker::hasTrips( const Player* player ) const
{
	std::vector<int> dupInd = dupRankInd( player );
	return dupInd.size() == 3;
}

Cards Poker::getTrips( const Player* player ) const
{
	std::vector<Card> cards;
	if ( !hasTrips( player ) )
	{
		throw poker_error::HandIDError( "HandIDError-->getTrips()" );
	}
	else
	{
		std::vector<int> dupInd = dupRankInd( player );
		auto i = dupInd.begin();
		while ( i != dupInd.end() )
		{
			cards.push_back( player->hand[ *i ] );
			i++;
		}
	}
	return cards;
}

bool Poker::hasFourOfAKind( const Player* player ) const
{
	using std::count;
	std::vector<int> dupInd = dupRankInd( player );
	if ( dupInd.size() == 4 )
	{
		Card sampleCard = player->hand[ dupInd[ 0 ] ];

		return count( player->hand.begin(),
					  player->hand.end(), sampleCard ) == 4;
	}
	else
	{
		return false;
	}
}

Cards Poker::getFourOfAKind( const Player* player ) const
{
	std::vector<int> dupInd = dupRankInd( player );
	Cards cards;
	if ( hasFourOfAKind( player ) )
	{
		throw poker_error::HandIDError( "HandIDError-->getFourOfAKind()" );
	}
	else
	{
		auto i = dupInd.begin();

		while ( i != dupInd.end() )
		{
			cards.push_back( player->hand[ *i ] );
			i++;
		}
	}
	return cards;
}

bool Poker::hasStraight( const Player* player ) const
{
	Cards cards = player->hand;                 // copy hand and sort
	std::vector<std::string> suits = getHandSuits( player );
	std::sort( cards.begin(), cards.end() );
	int cnt = 0;
	for ( int i = 1; i < 5; i++ )
	{
		if ( cards[ i ].rankIndex - cards[ i - 1 ].rankIndex == 1 )
		{
			cnt++;
		}
	}
	return cnt == 4 && count( suits.begin(), suits.end(), suits[ 0 ] ) != 5;
}

Cards Poker::getStraight( const Player* player ) const
{
	if ( !hasStraight( player ) )
	{
		throw poker_error::HandIDError( "HandIDError-->getStraight()" );
	}
	else
	{
		// copy bc cannot change hand in player object do to constness
		Cards local_hand( player->hand );
		sort( local_hand.begin(), local_hand.end() );
		return local_hand;
	}
}

bool Poker::hasFlush( const Player* player ) const
{
	using std::count;
	std::vector<std::string> suits = getHandSuits( player );
	Cards cards = player->hand;
	std::sort( cards.begin(), cards.end() );
	int cnt = 0;
	for ( int i = 1; i < 5; i++ )
	{
		if ( cards[ i ].rankIndex - cards[ i - 1 ].rankIndex == 1 )
		{
			cnt++;
		}
	}
	// checking for a flush only and not a straight flush
	return count( suits.begin(), suits.end(), suits[ 0 ] ) == 5 && cnt != 4
		   && !hasFullHouse( player );
}

Cards Poker::getFlush( const Player* player ) const
{
	if ( !hasFlush( player ) )
	{
		throw poker_error::HandIDError( "HandIDError-->getFlush()" );
	}
	else
	{
		// copy due to constness
		Cards local_hand( player->hand );
		sort( local_hand.begin(), local_hand.end() );
		return local_hand;
	}
}

bool Poker::hasFullHouse( const Player* player ) const
{
	using std::count;
	Cards cards = player->hand;
	if ( cards.size() != 5 )
	{
		throw poker_error::HandIDError( "Error: @hasFullHouse -->hand size not 5 cards" );
	}
	sort( cards.begin(), cards.end() );
	int a = count( cards.begin(), cards.end(), cards[ 0 ] );        // either 2, 3, or 0
	int b = count( cards.begin(), cards.end(), cards[ 4 ] );        // either 2, 3, or 0
	return ( a + b ) == 5;      // if a pair and trips in one hand then we have a fullHouse
}

Cards Poker::getFullHouse( const Player* player ) const
{

	if ( !hasFullHouse( player ) )
	{
		throw poker_error::HandIDError( "Error: HandIDError --> "
												"@getFullHouse(), player does not have a full house!" );
	}
	else
	{
		// copy due to constness
		Cards local_hand( player->hand );
		sort( local_hand.begin(), local_hand.end() );
		return local_hand;
	}
}

bool Poker::hasStraightFlush( const Player* player ) const
{
	Cards cards = player->hand;
	std::vector<std::string> suits = getHandSuits( player );
	std::sort( cards.begin(), cards.begin() );
	int rnkCnt = 0, stCnt = 0;
	for ( int i = 1; i < 5; i++ )
	{
		if ( cards[ i ].rankIndex - cards[ i - 1 ].rankIndex == 1 )
		{
			rnkCnt++;
		}
	}
	stCnt = std::count( suits.begin(), suits.end(), suits[ 0 ] );
	return stCnt == 5 && rnkCnt == 4 && cards[ 4 ].getRank() != "A";
}

Cards Poker::getStraightFlush( const Player* player ) const
{
	if ( !hasStraightFlush( player ) )
	{
		throw poker_error::HandIDError( "Error: @getStraightFlush()-->HandIDError--> "
												"player does not have a Straight Flush" );
	}
	else
	{
		return player->hand;
	}
}

bool Poker::hasRoyalFlush( const Player* player ) const
{
	Cards cards = player->hand;
	std::vector<std::string> suits = getHandSuits( player );
	sort( cards.begin(), cards.end() );
	int rnkCnt = 0, stCnt = 0;
	for ( int i = 1; i < 5; i++ )
	{
		if ( cards[ i ].rankIndex - cards[ i - 1 ].rankIndex == 1 )
		{
			rnkCnt++;
		}
	}
	stCnt = std::count( suits.begin(), suits.end(), suits[ 0 ] );
	return stCnt == 5 && rnkCnt == 4 && cards[ 4 ].getRank() == "A";
}

Cards Poker::getRoyalFlush( const Player* player ) const
{
	if ( !hasRoyalFlush( player ) )
	{
		throw poker_error::HandIDError( "HandIDError: @getRoyalFlush -->"
												" Player does not have royal flush" );
	}
	else
	{
		return player->hand;
	}
}

std::vector<std::string> Poker::getHandSuits( const Player* player ) const
{
	std::vector<std::string> suits;
	for ( const Card& cd: player->hand )
	{
		suits.push_back( cd.getSuit() );
	}
	return suits;
}

Player& Poker::getPlayerRef( int index )
{
	// returns a reference to a player's reference by index
	return players[ index ];
}

Player& Poker::getPlayerRef( std::string& name )
{
	for ( auto& plyr : players )
	{
		if ( plyr.name == name )
		{
			return plyr;
		}
	}
	throw poker_error::FindPlayerError();
}
