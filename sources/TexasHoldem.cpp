#include "TexasHoldem.h"
#include <fstream>
using namespace std;


TexasHoldem::TexasHoldem()
	:  Poker(), pot(0), smallBlind(10), bigBlind(20), gameId(999), findHandFlag(0) {
	// Constructor that inits from configuration
	cout << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"<< endl;
	cout << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" <<endl;
	for(auto p: players)
		cout << p << endl;
	assign_dealer();
}

TexasHoldem::TexasHoldem(string& loadPath)
	: Poker(), pot(0), findHandFlag(0) {

	// Init game from supplied config file
	cout << "TexasHoldem:: Game init via " << loadPath << endl;
	ifstream file(loadPath);
	Json::Value 	root, game;
	file >> root;

	cout << "cfg file read:: " << endl;
	cout << root << endl;
	Json::Value 	def_val( -1 );
	
	gameId 		= root["game"]["id"].asInt(); //root.get( key, def_val );
	cout << "game CFG::Game Id: " << gameId << endl;
	
	string key 	= "small_blind";
	def_val  	= 10;
	smallBlind 	= root["game"].get( key, def_val ).asInt();
	cout << "game CFG::smallBlind:: " << smallBlind << endl;

	key 		= "big_blind";
	def_val 	= 20;
	bigBlind 	= root["game"].get( key, def_val ).asInt();
	cout << "game CFG::bigBlind:: " << bigBlind << endl;

	Json::Value player_array;
	player_array =root["game"]["players"];
	if( !player_array.isArray() ){
		// take some sort of contingency 
		cerr << "Error reading players from configurations, exiting" << endl;
		return;
	}

	Json::Value::iterator it = player_array.begin();
	string player_name;
	numPlayers = root["game"]["number_of_players"].asInt();
	while( it != player_array.end() ){
		player_name = (*it)["name"].asString();
		Player plyr(player_name, (*it)["starting_cash"].asInt());
		players.push_back(plyr);
		cout << "Created player: " + player_name << endl;
		it++;
	}
	// TODO:: finish here getting the player and configuring them as well
	assign_dealer();
}

TexasHoldem::TexasHoldem(int smallBlind, int bigBlind, int cash)
	: Poker(cash), pot(0), smallBlind(smallBlind), bigBlind(bigBlind), gameId(999), findHandFlag(0) {

	cout << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"<< endl;
	cout << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" <<endl;
	for(auto p: players)
		cout << p << endl;
	assign_dealer();
}

TexasHoldem::TexasHoldem(std::initializer_list<std::pair<string, int>> list)
	:  Poker(list), pot(0), smallBlind(10), bigBlind(20), gameId(999), findHandFlag(0) {

	cout << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"<< endl;
	cout << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" <<endl;
	for(auto p: players)
		cout << p << endl;
	assign_dealer();
}

TexasHoldem::TexasHoldem(std::initializer_list<string> names)
	: Poker(names), pot(0), smallBlind(10), bigBlind(20), gameId(999), findHandFlag(0) {

	cout << "\n-----------Welcome!!!----------------\n New Texas Holdem game created!!!\n"<< endl;
	cout << "Small Blind: " << smallBlind << "\tBig Blind: " << bigBlind << "\n\nPlayers:\n" <<endl;
	for(auto p: players)
		cout << p << endl;
	assign_dealer();
}

void TexasHoldem::showHand(const Player* player) const{
	cout << *player << endl;

	for(Card cd : player->hand){
		cout << cd << endl;
	}
}

void TexasHoldem::dealHands(){
	using std::move;
	for(auto i = 0; i < 2; i++){
		for(auto& player : players)
			player.hand.push_back( move( gameDeck.dealCard() ) );
	}
	cout << "cards dealt out to players.... \n\n" << endl;
}

void TexasHoldem::changeBlinds(int sm, int big){
	smallBlind = sm;
	bigBlind = big;

	cout << "Small blind has been increased to $" << smallBlind << endl;
	cout << "Big blind has been increased to $" << bigBlind << "\n" << endl;
}

void TexasHoldem::showTableCards() const{
	if(tableCards.size() == 0){
		cout << " No table cards ....\n\n " << endl;
	}
	else{
		cout << "Table Cards: " << endl;

		for(auto cd : tableCards)
			cout << cd << endl;
	}
}

void TexasHoldem::dealFlop(){
	gameDeck.dealCard();

	for(int i = 0; i < 3; i++)
		tableCards.push_back(gameDeck.dealCard());

	cout << "Flop has been dealt....\n" << endl;
}

void TexasHoldem::dealTurn(){
	gameDeck.dealCard();
	tableCards.push_back(gameDeck.dealCard());
	cout << "Turn card has been dealt....\n" << endl;
}

void TexasHoldem::dealRiver(){
	gameDeck.dealCard();
	tableCards.push_back(gameDeck.dealCard());
	cout << "River card has been dealt...\n" << endl;
}

void TexasHoldem::playerBet(Player* plyr, int amt){
	plyr->bet(amt);
	pot+=amt;
	cout << "Pot is now $" << pot << endl;
}

void TexasHoldem::betSmallBlind(Player* plyr){
	plyr->bet(smallBlind);
	pot += smallBlind;
	cout << "Pot is now $" << pot << endl;
}

void TexasHoldem::betBigBlind(Player* plyr){
	plyr->bet(bigBlind);
	pot += bigBlind;
	cout << "Pot is now $" << pot << endl;
}

void TexasHoldem::findHand(Player* plyr){
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

	if(tableCards.size() != 5 || plyr->hand.size() != 2)
		throw poker_error::HandIDError("Error--> TexasHoldem:findHand():: 7 cards not present");
	
	Cards allCards = tableCards;
	cardSuperVector possHands = comboCards(plyr);

	Card hole1 = plyr->hand[0];
	Card hole2 = plyr->hand[1];
	allCards.push_back(hole1);
	allCards.push_back(hole2);
	sort(allCards.begin(), allCards.end());
	plyr->bestHand.clear();
	plyr->clearHand();
	plyr->clearHandStrength();
	int handsChecked = 0;
	std::string message("error");

	for(Cards possHand: possHands){
		handsChecked++;
		plyr->hand = possHand;

        if(hasRoyalFlush(plyr)){
			int tmp = HandMapper::handMap.at("royalFlush")[0];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Royal Flush";
			}
		}
		else if(hasStraightFlush(plyr)){
			int highCardRank = plyr->bestHand[4].rankIndex;
			int tmp = HandMapper::handMap.at("straightFlush")[highCardRank];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Straight Flush"; 
			}
		}
		else if(hasFourOfAKind(plyr)){
			int cardRnk = getFourOfAKind(plyr)[0].rankIndex;
			int tmp = HandMapper::handMap.at("fourOfAKind")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;

				if(cardRnk == allCards[6].rankIndex)
					plyr->highCardRnk = allCards[2].rankIndex;  // c0 c1 c2 c3 c4 c5 c6-->
				else 
					plyr->highCardRnk = allCards[6].rankIndex;
				message = "a Four of a Kind";
			}
		}
		else if(hasFullHouse(plyr)){
			Cards fullHouse = getFullHouse(plyr);
			int cardRnk;

			if(count(fullHouse.begin(), fullHouse.end(), fullHouse[0]) == 3)
				cardRnk = fullHouse[0].rankIndex;
			else
				cardRnk = fullHouse[4].rankIndex;

			int tmp = HandMapper::handMap.at("fullHouse")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Full House";
			}
		}
		else if(hasFlush(plyr)){
			Cards flush = getFlush(plyr);
			int cardRnk = flush[4].rankIndex;
			int tmp = HandMapper::handMap.at("flush")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Flush";
			}
		}
		else if(hasStraight(plyr)){
			int cardRnk = getStraight(plyr)[4].rankIndex;
			int tmp = HandMapper::handMap.at("straight")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				plyr->highCardRnk = 0;
				message = "a Straight";
			}
		}
		else if(hasTrips(plyr)){
			int cardRnk =getTrips(plyr)[0].rankIndex;
			int tmp = HandMapper::handMap.at("trips")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;

				if(cardRnk == allCards[6].rankIndex)
					plyr->highCardRnk = allCards[3].rankIndex;				
				else 
					plyr->highCardRnk = allCards[6].rankIndex;
					
					message = "a Three of a Kind";
			}
		}
		else if(hasTwoPair(plyr)){
			Cards twoPair = getTwoPair(plyr);
            int cardRnk = twoPair[3].rankIndex;
            int tmp = HandMapper::handMap.at("twoPair")[cardRnk];

            if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;

				for(int i = allCards.size()-1; i >= 0; --i){
					
					if(allCards[i].rankIndex != twoPair[3].rankIndex\
					 && allCards[i].rankIndex != twoPair[0].rankIndex){
						plyr->highCardRnk = allCards[i].rankIndex;
					}
				}
				//	plyr->highCardRnk = allCards[6].rankIndex;
				message = "Two Pair";
			}
		}
		else if(hasPair(plyr)){
			int cardRnk = getPair(plyr)[0].rankIndex;
			int tmp = HandMapper::handMap.at("pair")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;

				if(allCards[6].rankIndex != cardRnk)
					plyr->highCardRnk = allCards[6].rankIndex;
				else
					plyr->highCardRnk = allCards[4].rankIndex;
				message = "a Pair";

			}
		}
		else{
			sort(plyr->hand.begin(), plyr->hand.end());
			int cardRnk = plyr->hand[4].rankIndex;
			int tmp = HandMapper::handMap.at("high")[cardRnk];

			if(tmp > plyr->handStrength){
				plyr->bestHand = plyr->hand;
				plyr->handStrength = tmp;
				message = "a High Card";
				plyr->highCardRnk = 0;
			}
		}
	}	

	plyr->hand.clear();
	cout << plyr->name <<" has " << message << " with a hand strength of " <<
				plyr->handStrength << " with a high card value of " << plyr->highCardRnk << endl;
	plyr->hand.push_back(hole1);
	plyr->hand.push_back(hole2);
	findHandFlag = true;

	for(auto p : players){

		if(p.handStrength == 0)
			findHandFlag = false;
	}
}

void TexasHoldem::findWinner(){
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

	if(!findHandFlag)	// ensures all players have had their hand strengths set
		throw poker_error::FindWinnerError();

	sort(players.begin(), players.end(), HandComparator()); //TODO: may change player order
	// debug line: to check if players were sorted properly 
	for(Player plyr : players){

		cout <<"Debug@TexasHoldem::findWinner->handStrengths " << plyr.handStrength << endl; 
	}

	// continue with find winner logic [. . . . . .]
	int cnt = 0;
	int maxInd = players.size()-1;
	for(int i = 1; i <= maxInd; i++){
		if(players[maxInd].handStrength == players[maxInd-i].handStrength)
			cnt++;
	}
	// debug 
	cout << "Count of ties: " << cnt << endl; 

	vector<Player> tiedPlayers;
	if(cnt > 0){
		for(int i = 0; i <= cnt; i++)
			tiedPlayers.push_back(players[maxInd-i]);
	}
	else{
		cout << players[maxInd].name << " has won the hand!!" << endl;
		players[maxInd].collectPot(pot); 
		findHandFlag = false;
		return;
	}

	// debug line 
	cout << "Debug@TexasHoldem::findWinner->tiedPlayers.size()::::: " << tiedPlayers.size() << endl; 
	
	// break tie if present using tiedPlayers vector

	sort(tiedPlayers.begin(), tiedPlayers.end(), HighCardRnkComparator());
	// still need logic to sort out tied players below 
	int stillTied = 0;
	
	if(tiedPlayers[tiedPlayers.size() -2].highCardRnk ==\
												tiedPlayers[tiedPlayers.size()-1].highCardRnk){
		
		for(auto p : tiedPlayers){
			if(p.highCardRnk == tiedPlayers[tiedPlayers.size()-1].highCardRnk){
				stillTied++;	// count of how many people are tied
			}
		}
		// debug line 
		cout << "We have " << stillTied << " players " << endl; 

		vector<string> winningNames;
		for(int i = 0; i < stillTied; ++i){	// [ * * * * *]  say stillTied = 3; loop goes 0 1 2 
			winningNames.push_back(tiedPlayers[tiedPlayers.size()-1-i].name);
		} 
		int numTies = winningNames.size();
		int amtAwarded = pot / numTies ; 			// to split pot amongst ppl involved in the tie

		cout << "Pot must be split amongst " << numTies << " players\n" << endl;
		for(auto name : winningNames){				// award winners split pot
			Player* winner = findPlayer(name);
			winner->collectPot(amtAwarded);
		}
	}
	else{
		// winner of hand when tie is broken
		string winnerName(tiedPlayers[tiedPlayers.size()-1].name);
		Player* winner = findPlayer(winnerName);
		winner->collectPot(pot);
	}
	
	// end of method findWinner 
	findHandFlag = false;
	return; 
}

Player* TexasHoldem::findPlayer(const string name) { // TODO:::::needs testing
	// private method that returns pointer to player identified by the name
	Player* plyr = nullptr;
	for(unsigned int i = 0; i < players.size(); i++){
		if(players[i].name == name){
			plyr = &players[i];
			break;
		}
	}
	return plyr;
}

cardSuperVector TexasHoldem::comboCards(const Player* plyr) const{
	cardSuperVector result;
	Cards cardSubSet;
	intComb combInd;
	Cards allCards = tableCards;

	for(auto cd: plyr->hand)
		allCards.push_back(cd);

	if(allCards.size() != 7)
		throw poker_error::HandIDError("Error @ comboCards(): 7 cards not present...");

	unsigned long numCombos = nCr(7, 5);	    
	combInd = comb(7, 5);

	for(vector<int> subIndVect: combInd){

		for(int index: subIndVect){
			cardSubSet.push_back(allCards[index]);
		}
		result.push_back(cardSubSet);
		cardSubSet.clear();
	}

	if(result.size() != numCombos)
		throw poker_error::HandIDError("Error @ comboCards(): 21 combos not present in hand combos");

	return result;
}

void TexasHoldem::resetHand(int num){
	pot = 0;
	gameDeck.resetDeck();
	tableCards.clear();

	for(unsigned int i = 0; i < players.size(); i++){
		players[i].reset();
	}	

    cout << "======================hand reset!!=======#"<< num << "==================\n\n" << endl;
}

intComb comb(int n, int k){
	intComb result;
	vector<int> maskVect(k, 1);
	maskVect.resize(n, 0);
	vector<int> subVect;
	int cnt = 1;

	do{
		for (int i = 0; i < n; ++i){
			if(maskVect[i]){
				subVect.push_back(i);
			}
		}
		result.push_back(subVect);
		subVect.clear();
		cnt++;
	}while(prev_permutation(maskVect.begin(), maskVect.end()));

	return result;
}

long nCr(int n, int k){
	 long x = fact(n);
	 long y = fact(k)*fact(n-k);
	return x/y;

}

long fact(int n){
	int tmp = n-1;

	while(tmp > 1){
		n *= tmp;
		tmp -= 1;
	}
	return n;
}

void debugPrintVect(vector<int> vect){

    cout << "Current vector values: " ;

    for(auto i: vect)
        cout << i << " ";
    cout << "\nEnd debug print\n\n" << endl;
}

void TexasHoldem::collectPot(Player* plyr){
	plyr->collectPot(pot);
}

void TexasHoldem::gameStatus() const{

	cout << "=========================Game Status===================================================" << endl;
	cout << "\tSmall Blind: " << smallBlind << "\tBig Blind: " << bigBlind << endl; 
	cout << "\tPot: " << pot << endl;

	for(auto plyr : players){
		showHand(&plyr);
	}

	showTableCards();
}

void TexasHoldem::gameOverview() const{
	cout << "\n\n=========================Game Overview=============================================" << endl;
	cout << "Game ID: " << gameId << endl;
	cout << "Small Blind: " << smallBlind << endl;
	cout << "Big Blind: " << bigBlind << "\n" << endl;
	
	for ( auto& plyr : players ) {
		cout << plyr << endl;
	}
	cout << "=======================================================================================" << endl;
}

void TexasHoldem::assign_dealer(){
	// function to assign dealer at beginning of game
	// helper function to be called in the ctor
	cout << "NumPlayers: " << numPlayers << endl;
	srand(time(NULL));
	uint32_t index_player = rand() % numPlayers;
	cout << "Index of player: " << index_player << endl;
	getPlayerRef(index_player).dealer = true;
	dealerIndex = index_player;

	if( index_player >= (numPlayers-1) ){
		index_player = 0;
		dealerIndex = 0;
		getPlayerRef(index_player).smallBlind_bet = true;
		getPlayerRef(++index_player).bigBlind_bet = true;
	}
	else{
		getPlayerRef(++index_player).smallBlind_bet = true;
		dealerIndex = index_player;	  // next hands dealer
		if ( index_player >= (numPlayers-1) ){
			getPlayerRef(0).bigBlind_bet = true;
		}
		else{
			getPlayerRef(index_player).bigBlind_bet = true;
		}
	}
	gameOverview();
}

void TexasHoldem::rotateDealer(){
	// rotates to next dealer, small and big blind players
	for( auto& plyr : players ){
		plyr.dealer 		= false;
		plyr.smallBlind_bet = false;
		plyr.bigBlind_bet 	= false;
	}
	cout << "Dealer Index: " << dealerIndex << endl;
	getPlayerRef(dealerIndex++).dealer = true;

	if(dealerIndex >= numPlayers-1){
		dealerIndex = 0;

		cout << "smallBlind Index: " << dealerIndex << endl;
		getPlayerRef(dealerIndex++).smallBlind_bet = true;

		cout << "bigBlind Index: " << dealerIndex << endl;
		getPlayerRef(dealerIndex).bigBlind_bet = true;
	}
	else{
		int tmp = dealerIndex;

		cout << "smallBlind Index: " << dealerIndex << endl;
		getPlayerRef(dealerIndex++).smallBlind_bet = true;

		cout << "smallBlind Index: " << dealerIndex << endl;
		getPlayerRef(dealerIndex).bigBlind_bet = true;
		dealerIndex = tmp;
	}
}
