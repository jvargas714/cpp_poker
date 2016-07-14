
#ifndef HEADERS_BRAIN_H
#define HEADERS_BRAIN_H
#include "master_poker.h"

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
	virtual void assessFlopScenario( const Cards& cds )=0;
	virtual void assessTurnScenario( const Cards& cds )=0;
	virtual void assessRiverScenario( const Carfds& cds )=0; 
};


/* Brain that will utilize pot odds */
class PotBrain : public Brain 
{
	PotBrain();
	~PotBrain();
};


#endif /* END HEADERS_BRAIN_H */