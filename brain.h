#ifndef HEADERS_BRAIN_H
#define HEADERS_BRAIN_H

#include <map>
#include "TexasHoldem.h"
#include "poker_fsm.h"
#include "assessment.h"
#include "bot_player.h"

/*
	Abstract class that will characterize bot_player's autonomous behavior
	REF: http://www.thepokerbank.com/strategy/mathematics/pot-odds/

	Design is to have a brain class type wrapped in a bot player. Betting functions will wrap
	brain functionality.
*/

class Brain
{
public:
	Brain();

	virtual ~Brain();

	bool assessTable( Player* bot, poker_states state );

	virtual void bet()=0;

protected:
	TexasHoldem* game;

	virtual void init( TexasHoldem* pkr )=0;

	virtual bool assessFlopScenario( Player* bot )=0;

	virtual bool assessTurnScenario( Player* bot )=0;

	virtual bool assessRiverScenario( Player* bot )=0;

	virtual bool assessTable( const Cards& cds );
};

//--------------------------------------------------------------------------------------------------

/* Brain that will utilize pot odds */
class PotBrain : public Brain
{
public:
	PotBrain();

	~PotBrain();

protected:
	bool assessFlopScenario( Player* bot );

	bool assessTurnScenario( Player* bot );

	bool assessRiverScenario( Player* bot );
};


//--------------------------------------------------------------------------------------------------


/* Brain that simply uses ideas from me REF:: my brain */
class JayBrain : public Brain
{
public:
	JayBrain();

	JayBrain( TexasHoldem* pkr );

	~JayBrain();

protected:
	void init( TexasHoldem* pkr );

	bool assessFlopScenario( Player* bot );

	bool assessTurnScenario( Player* bot );

	bool assessRiverScenario( Player* bot );


};


//--------------------------------------------------------------------------------------------------
#endif /* END HEADERS_BRAIN_H */
