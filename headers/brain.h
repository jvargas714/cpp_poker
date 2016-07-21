
#ifndef HEADERS_BRAIN_H
#define HEADERS_BRAIN_H
#include "master_poker.h"
#include <map>

/*
	Abstract class that will characterize bot_player's autonomous behavior
	REF: http://www.thepokerbank.com/strategy/mathematics/pot-odds/
*/
class Brain
{
public:
	Brain();
	virtual ~Brain();
	void assessTable( const Cards& cds, poker_states state );
	virtual void bet()=0;

private:
	Poker* game;
	void init(Poker* pkr);
	virtual void assessFlopScenario( const Cards& cds, TexasHoldem& game )=0;
	virtual void assessTurnScenario( const Cards& cds, TexasHoldem& game )=0;
	virtual void assessRiverScenario( const Carfds& cds, TexasHoldem& game )=0; 
};



//--------------------------------------------------------------------------------------------------

/* Brain that will utilize pot odds */
class PotBrain : public Brain 
{
public:
	PotBrain();
	~PotBrain();

private:
	void assessFlopScenario( const Cards& cds, TexasHoldem& game );
	void assessTurnScenario( const Cards& cds, TexasHoldem& game );
	void assessRiverScenario( const Cards& cds TexasHoldem& game );
};


//--------------------------------------------------------------------------------------------------





/* Brain that simply uses ideas from me REF:: my brain */
class JayBrain : public Brain 
{
public:
	JayBrain();
	~JayBrain();

private:
	void assessFlopScenario( const Cards& cds, TexasHoldem& game );
	void assessTurnScenario( const Cards& cds, TexasHoldem& game );
	void assessRiverScenario( const Cards& cds TexasHoldem& game );
};


//--------------------------------------------------------------------------------------------------


#endif /* END HEADERS_BRAIN_H */