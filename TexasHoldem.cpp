#include "TexasHoldem.h"
#include "logger.h"
#include <random>

TexasHoldem::TexasHoldem()
		: Poker(), pot( 0 ), smallBlind( 10 ), bigBlind( 20 ), gameId( 999 )
{
	// Constructor that inits from configuration
	log() << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"
		  << std::endl;
	log() << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n"
		  << std::endl;
	for ( auto& p: players )
	{
		log() << p << std::endl;
	}
	assign_dealer();
}

TexasHoldem::TexasHoldem( string& loadPath )
		: Poker(), pot( 0 )
{

	// Init game from supplied config file
	log() << "TexasHoldem:: Game init via " << loadPath << std::endl;
	std::ifstream file( loadPath );
	Json::Value root, game;
	file >> root;
	log() << "cfg file read:: " << std::endl;
	log() << root << std::endl;
	Json::Value def_val( -1 );
	gameId = root[ "game" ][ "id" ].asInt(); //root.get( key, def_val );
	log() << "game CFG::Game Id: " << gameId << std::endl;
	string key = "small_blind";
	def_val = 10;
	smallBlind = root[ "game" ].get( key, def_val ).asInt();
	log() << "game CFG::smallBlind:: " << smallBlind << std::endl;
	key = "big_blind";
	def_val = 20;
	bigBlind = root[ "game" ].get( key, def_val ).asInt();
	log() << "game CFG::bigBlind:: " << bigBlind << std::endl;
	Json::Value player_array;
	player_array = root[ "game" ][ "players" ];
	if ( !player_array.isArray() )
	{
		// take some sort of contingency 
		std::cerr << "Error reading players from configurations, exiting" << std::endl;
		return;
	}
	Json::Value::iterator it = player_array.begin();
	std::string player_name;
	numPlayers = root[ "game" ][ "number_of_players" ].asInt();
	while ( it != player_array.end() )
	{
		player_name = ( *it )[ "name" ].asString();
		Player plyr( player_name, ( *it )[ "starting_cash" ].asInt() );
		players.push_back( plyr );
		log() << "Created player: " + player_name << std::endl;
		it++;
	}
	// TODO:: finish here getting the player and configuring them as well
	assign_dealer();
}

TexasHoldem::TexasHoldem( int smallBlind, int bigBlind, int cash )
		: Poker( cash ), pot( 0 ), smallBlind( smallBlind ), bigBlind( bigBlind ), gameId( 999 )
{
	log() << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"
		  << std::endl;
	log() << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n"
		  << std::endl;
	for ( auto& p: players )
	{
		log() << p << std::endl;
	}
	assign_dealer();
}

TexasHoldem::TexasHoldem( std::initializer_list<std::pair<std::string, int>> list )
		: Poker( list ), pot( 0 ), smallBlind( 10 ), bigBlind( 20 ), gameId( 999 )
{
	log() << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"
		  << std::endl;
	log() << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n"
		  << std::endl;
	for ( auto& p: players )
	{
		log() << p << std::endl;
	}
	assign_dealer();
}

TexasHoldem::TexasHoldem( std::initializer_list<std::string> names )
		: Poker( names ), pot( 0 ), smallBlind( 10 ), bigBlind( 20 ), gameId( 999 )
{
	log() << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"
		  << std::endl;
	log() << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n"
		  << std::endl;
	for ( auto& p: players )
	{
		log() << p << std::endl;
	}
	assign_dealer();
}

void TexasHoldem::showHand( const Player* player ) const
{
	log() << *player;
	for ( const Card& cd : player->hand )
	{
		log() << cd << std::endl;
	}
	log() << "\n";
}

void TexasHoldem::dealHands()
{
	using std::move;
	for ( auto i = 0; i < 2; i++ )
	{
		for ( auto& player : players )
		{
			player.hand.push_back( gameDeck.dealCard() );
		}
	}
	log() << "cards dealt out to players.... \n\n" << std::endl;
}

void TexasHoldem::changeBlinds( int sm, int big )
{
	smallBlind = sm;
	bigBlind = big;
	log() << "Small blind has been increased to $" << smallBlind << std::endl;
	log() << "Big blind has been increased to $" << bigBlind << "\n" << std::endl;
}

void TexasHoldem::showTableCards() const
{
	if ( tableCards.empty() )
	{
		log() << "No table cards ....\n\n " << std::endl;
	}
	else
	{
		log() << "Table Cards: " << std::endl;
		for ( auto& cd : tableCards )
		{
			log() << cd << std::endl;
		}
	}
}

void TexasHoldem::dealFlop()
{
	gameDeck.dealCard();
	for ( int i = 0; i < 3; i++ )
	{
		tableCards.push_back( gameDeck.dealCard() );
	}
	log() << "Flop has been dealt....\n" << std::endl;
}

void TexasHoldem::dealTurn()
{
	gameDeck.dealCard();
	tableCards.push_back( gameDeck.dealCard() );
	log() << "Turn card has been dealt....\n" << std::endl;
}

void TexasHoldem::dealRiver()
{
	gameDeck.dealCard();
	tableCards.push_back( gameDeck.dealCard() );
	log() << "River card has been dealt...\n" << std::endl;
}

void TexasHoldem::playerBet( Player* plyr, int amt )
{
	plyr->bet( amt );
	pot += amt;
	log() << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::betSmallBlind( Player* plyr )
{
	plyr->bet( smallBlind );
	pot += smallBlind;
	log() << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::betBigBlind( Player* plyr )
{
	plyr->bet( bigBlind );
	pot += bigBlind;
	log() << "Pot is now $" << pot << std::endl;
}

void TexasHoldem::findHand( Player* plyr )
{
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
	using std::count;
	if ( tableCards.size() != 5 || plyr->hand.size() != 2 )
	{
		throw poker_error::HandIDError( "Error--> TexasHoldem:findHand():: 7 cards not present" );
	}
	Cards allCards = tableCards;
	cardSuperVector possHands = comboCards( plyr );
	Card hole1 = plyr->hand[ 0 ];
	Card hole2 = plyr->hand[ 1 ];
	allCards.push_back( hole1 );
	allCards.push_back( hole2 );
	sort( allCards.begin(), allCards.end() );
	plyr->bestHand.clear();
	plyr->clearHand();
	plyr->clearHandStrength();
	std::string message( "error" );
	for ( Cards& possHand: possHands )
	{
		plyr->hand = possHand;
		if ( hasRoyalFlush( plyr ) )
		{
			int tmp = HandMapper::handMap.at( "royalFlush" )[ 0 ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Royal Flush";
			}
		}
		else if ( hasStraightFlush( plyr ) )
		{
			int highCardRank = plyr->bestHand[ 4 ].rankIndex;
			int tmp = HandMapper::handMap.at( "straightFlush" )[ highCardRank ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Straight Flush";
			}
		}
		else if ( hasFourOfAKind( plyr ) )
		{
			int cardRnk = getFourOfAKind( plyr )[ 0 ].rankIndex;
			int tmp = HandMapper::handMap.at( "fourOfAKind" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				if ( cardRnk == allCards[ 6 ].rankIndex )
				{
					plyr->highCardRnk = allCards[ 2 ].rankIndex;  // c0 c1 c2 c3 c4 c5 c6-->
				}
				else
				{
					plyr->highCardRnk = allCards[ 6 ].rankIndex;
				}
				message = "a Four of a Kind";
			}
		}
		else if ( hasFullHouse( plyr ) )
		{
			Cards fullHouse = getFullHouse( plyr );
			int cardRnk;
			if ( count( fullHouse.begin(), fullHouse.end(), fullHouse[ 0 ] ) == 3 )
			{
				cardRnk = fullHouse[ 0 ].rankIndex;
			}
			else
			{
				cardRnk = fullHouse[ 4 ].rankIndex;
			}
			int tmp = HandMapper::handMap.at( "fullHouse" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Full House";
			}
		}
		else if ( hasFlush( plyr ) )
		{
			Cards flush = getFlush( plyr );
			int cardRnk = flush[ 4 ].rankIndex;
			int tmp = HandMapper::handMap.at( "flush" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Flush";
			}
		}
		else if ( hasStraight( plyr ) )
		{
			int cardRnk = getStraight( plyr )[ 4 ].rankIndex;
			int tmp = HandMapper::handMap.at( "straight" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Straight";
			}
		}
		else if ( hasTrips( plyr ) )
		{
			int cardRnk = getTrips( plyr )[ 0 ].rankIndex;
			int tmp = HandMapper::handMap.at( "trips" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				if ( cardRnk == allCards[ 6 ].rankIndex )
				{
					plyr->highCardRnk = allCards[ 3 ].rankIndex;
				}
				else
				{
					plyr->highCardRnk = allCards[ 6 ].rankIndex;
				}
				message = "a Three of a Kind";
			}
		}
		else if ( hasTwoPair( plyr ) )
		{
			Cards twoPair = getTwoPair( plyr );
			int cardRnk = twoPair[ 3 ].rankIndex;
			int tmp = HandMapper::handMap.at( "twoPair" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				for ( size_t i = allCards.size() - 1; i != 0; --i )
				{
					if ( allCards[ i ].rankIndex != twoPair[ 0 ].rankIndex &&
						 allCards[ i ].rankIndex != twoPair[ 3 ].rankIndex\
 )
					{
						plyr->highCardRnk = allCards[ i ].rankIndex;
					}
				}
				message = "Two Pair";
			}
		}
		else if ( hasPair( plyr ) )
		{
			int cardRnk = getPair( plyr )[ 0 ].rankIndex;
			int tmp = HandMapper::handMap.at( "pair" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				if ( allCards[ 6 ].rankIndex != cardRnk )
				{
					plyr->highCardRnk = allCards[ 6 ].rankIndex;
				}
				else
				{
					plyr->highCardRnk = allCards[ 4 ].rankIndex;
				}
				message = "a Pair";
			}
		}
		else
		{
			sort( plyr->hand.begin(), plyr->hand.end() );
			int cardRnk = plyr->hand[ 4 ].rankIndex;
			int tmp = HandMapper::handMap.at( "high" )[ cardRnk ];
			if ( tmp > plyr->handStrength )
			{
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				message = "a High Card";
				plyr->highCardRnk = 0;
			}
		}
	}
	plyr->hand.clear();
	log() << plyr->name << " has " << message << " with a hand strength of " <<
		  plyr->handStrength << " with a high card value of " << plyr->highCardRnk << std::endl;
	plyr->hand.push_back( hole1 );
	plyr->hand.push_back( hole2 );
}

void TexasHoldem::calculateHands()
{
	for ( auto& plyr : players )
	{
		findHand( &plyr );
	}
}

void TexasHoldem::findWinner()
{
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

	// ensure all players have assessed there hand strengths
	for ( auto& p : players )
	{
		if ( p.handStrength == 0 )
		{
			findHand( &p );
		}
	}
	// put players in acesceding order iaw their handstrengths
	sort( players.begin(), players.end(), HandComparator() ); //TODO: may change player order
	// check player order
	for ( Player plyr : players )
	{
		log() << "TexasHoldem::findWinner(): handStrengths " << plyr.handStrength
			  << std::endl;
	}
	// continue with find winner logic [. . . . . .]
	int cnt = 0;
	size_t maxInd = players.size() - 1;
	for ( size_t i = 1; i <= maxInd; i++ )
	{
		if ( players[ maxInd ].handStrength == players[ maxInd - i ].handStrength )
		{
			cnt++;
		}
	}
	log() << "TexasHoldem::findWinner(): Count of ties: " << cnt << std::endl;
	std::vector<Player> tiedPlayers;
	if ( cnt > 0 )
	{
		for ( int i = 0; i <= cnt; i++ )
		{
			tiedPlayers.push_back( players[ maxInd - i ] );
		}
	}
	else
	{
		log() << "TexasHoldem::findWinner(): "
			  << players[ maxInd ].name << " has won the hand!!" << std::endl;
		players[ maxInd ].collectPot( pot );
		return;
	}

	// debug line 
	log() << "TexasHoldem::findWinner(): tiedPlayers.size -->  " << tiedPlayers.size()
		  << std::endl;
	// break tie if present using tiedPlayers vector
	sort( tiedPlayers.begin(), tiedPlayers.end(), HighCardRnkComparator() );
	int stillTied = 0;

	// if we still have a tie then split pot amongst the winners
	if ( tiedPlayers[ tiedPlayers.size() - 2 ].highCardRnk == \
                                                tiedPlayers[ tiedPlayers.size() - 1 ].highCardRnk )
	{
		for ( auto p : tiedPlayers )
		{
			if ( p.highCardRnk == tiedPlayers[ tiedPlayers.size() - 1 ].highCardRnk )
			{
				stillTied++;    // count of how many people are tied
			}
		}
		// debug line 
		log() << "We have " << stillTied << " players " << "still tied." << std::endl;
		std::vector<std::string> winningNames;
		for ( int i = 0; i < stillTied; ++i )
		{    // [ * * * * *]  say stillTied = 3; loop goes 0 1 2
			winningNames.push_back( tiedPlayers[ tiedPlayers.size() - 1 - i ].name );
		}
		size_t numTies = winningNames.size();
		size_t amtAwarded =
				pot / numTies;            // to split pot amongst ppl involved in the tie

		log() << "Pot must be split amongst " << numTies << " players\n" << std::endl;
		for ( auto& name : winningNames )
		{                // award winners split pot
			Player* winner = findPlayer( name );
			winner->collectPot( (int)amtAwarded );
		}
	}
	else
	{
		// winner of hand when tie is broken
		std::string winnerName( tiedPlayers[ tiedPlayers.size() - 1 ].name );
		Player* winner = findPlayer( winnerName );
		winner->collectPot( pot );
	}
}

Player* TexasHoldem::findPlayer( const std::string& name )
{
	// private method that returns pointer to player identified by the name
	for ( auto& player : players )
	{
		if ( player.name == name )
		{
			return &player;
		}
	}
	return nullptr;
}

cardSuperVector TexasHoldem::comboCards( const Player* plyr ) const
{
	cardSuperVector result;
	Cards cardSubSet;
	intComb combInd;
	Cards allCards = tableCards;
	for ( auto& cd: plyr->hand )
	{
		allCards.push_back( cd );
	}
	if ( allCards.size() != 7 )
	{
		throw poker_error::HandIDError( "Error @ comboCards(): 7 cards not present..." );
	}
	uint64_t numCombos = nCr( 7, 5 );
	combInd = comb( 7, 5 );
	for ( std::vector<int> subIndVect: combInd )
	{
		for ( int index: subIndVect )
		{
			cardSubSet.push_back( allCards[ index ] );
		}
		result.push_back( cardSubSet );
		cardSubSet.clear();
	}
	if ( result.size() != numCombos )
	{
		throw poker_error::HandIDError(
				"Error @ comboCards(): 21 combos not present in hand combos" );
	}
	return result;
}

void TexasHoldem::resetHand( int num )
{
	pot = 0;
	gameDeck.resetDeck();
	tableCards.clear();
	for ( auto& player : players )
	{
		player.reset();
	}
	log() << "======================hand reset!!=======#" << num << "==================\n\n"
		  << std::endl;
}

intComb comb( int n, int k )
{
	intComb result;
	std::vector<int> maskVect( (uint32_t)k, 1 );
	maskVect.resize( (size_t)n, 0 );
	std::vector<int> subVect;
	int cnt = 1;
	do
	{
		for ( int i = 0; i < n; ++i )
		{
			if ( maskVect[ i ] )
			{
				subVect.push_back( i );
			}
		}
		result.push_back( subVect );
		subVect.clear();
		cnt++;
	} while ( prev_permutation( maskVect.begin(), maskVect.end() ) );
	return result;
}

uint64_t nCr( uint64_t n, uint64_t k )
{
	uint64_t x = fact( n );
	uint64_t y = fact( k ) * fact( n - k );
	return x / y;
}

uint64_t fact( uint64_t n )
{
	uint64_t tmp = n - 1;
	while ( tmp > 1 )
	{
		n *= tmp;
		tmp -= 1;
	}
	return n;
}

void debugPrintVect( std::vector<int> vect )
{
	log() << "Current vector values: ";
	for ( auto i: vect )
	{
		log() << i << " ";
	}
	log() << "\nEnd debug print\n\n" << std::endl;
}

void TexasHoldem::collectPot( Player* plyr )
{
	plyr->collectPot( pot );
}

void TexasHoldem::gameStatus() const
{
	log()
			<< "=========================Game Status================================================"
			<< std::endl;
	log() << "Small Blind: " << smallBlind << "\nBig Blind: " << bigBlind << std::endl;
	log() << "Pot: " << pot << "\n" << std::endl;
	for ( auto plyr : players )
	{
		showHand( &plyr );
	}
	showTableCards();
	log() << "====================================================================================="
		  << std::endl;

}

void TexasHoldem::gameOverview() const
{
	log() << "\n\n=========================Game Overview=========================================="
		  << std::endl;
	log() << "Game ID: " << gameId << std::endl;
	log() << "Small Blind: " << smallBlind << std::endl;
	log() << "Big Blind: " << bigBlind << "\n" << std::endl;
	std::ostringstream ss;
	for ( auto& plyr : players )
	{
		ss << plyr << "\n";
	}
	log() << ss.str();
	log()
			<< "===================================================================================="
			<< std::endl;
}

void TexasHoldem::assign_dealer()
{
	// function to assign dealer at beginning of game
	// helper function to be called in the ctor
	log() << "NumPlayers: " << numPlayers << std::endl;
	std::random_device rd;
	int index_player = rd() % numPlayers;
	log() << "Index of player: " << index_player << std::endl;
	getPlayerRef( index_player ).dealer = true;
	dealerIndex = (uint32_t)index_player;
	if ( index_player >= ( numPlayers - 1 ) )
	{
		index_player = 0;
		dealerIndex = 0;
		getPlayerRef( index_player ).smallBlind_bet = true;
		getPlayerRef( ++index_player ).bigBlind_bet = true;
	}
	else
	{
		getPlayerRef( ++index_player ).smallBlind_bet = true;
		dealerIndex = (uint32_t)index_player;      // next hands dealer
		if ( index_player >= ( numPlayers - 1 ) )
		{
			getPlayerRef( 0 ).bigBlind_bet = true;
		}
		else
		{
			getPlayerRef( index_player ).bigBlind_bet = true;
		}
	}
}

void TexasHoldem::rotateDealer()
{
	// rotates to next dealer, small and big blind players
	for ( auto& plyr : players )
	{
		plyr.dealer = false;
		plyr.smallBlind_bet = false;
		plyr.bigBlind_bet = false;
	}
	log() << "Dealer Index: " << dealerIndex << std::endl;
	getPlayerRef( dealerIndex++ ).dealer = true;
	if ( dealerIndex >= numPlayers - 1 )
	{
		dealerIndex = 0;
		log() << "smallBlind Index: " << dealerIndex << std::endl;
		getPlayerRef( dealerIndex++ ).smallBlind_bet = true;
		log() << "bigBlind Index: " << dealerIndex << std::endl;
		getPlayerRef( dealerIndex ).bigBlind_bet = true;
	}
	else
	{
		int tmp = dealerIndex;
		log() << "smallBlind Index: " << dealerIndex << std::endl;
		getPlayerRef( dealerIndex++ ).smallBlind_bet = true;
		log() << "smallBlind Index: " << dealerIndex << std::endl;
		getPlayerRef( dealerIndex ).bigBlind_bet = true;
		dealerIndex = (uint32_t)tmp;
	}
}
