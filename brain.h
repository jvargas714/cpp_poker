#ifndef HEADERS_BRAIN_H
#define HEADERS_BRAIN_H

#include <map>
#include "TexasHoldem.h"
#include "poker_fsm.h"
#include "assessment.h"
#include "bot_player.h"
#include "logger.h"

/*
	Abstract class that will characterize bot_player's autonomous behavior
	REF: http://www.thepokerbank.com/strategy/mathematics/pot-odds/

	Design is to have a brain class type wrapped in a bot player. Betting functions will wrap brain functionality.
*/

class Brain
{
public:
	Brain();
	explicit Brain( Poker* );
	virtual ~Brain() = default;
	bool assessTable( Player* bot, poker_states state );
	virtual void bet()=0;

protected:
	Poker* game;  // may have to reconsider refactoring this class to work out,
					// and have the proper overrides in Texas Holdem class
	virtual bool assessFlopScenario( Player* bot )=0;
	virtual bool assessTurnScenario( Player* bot )=0;
	virtual bool assessRiverScenario( Player* bot )=0;
	virtual bool assessTable( const Cards& cds );
};

//----------------------------------------------------------------------------------------------------------------------

/* Brain that simply uses ideas from me REF:: my brain */
class JayBrain : public Brain
{
public:
	JayBrain();
	explicit JayBrain( TexasHoldem* pkr );
	~JayBrain() override = default;

protected:
	void init( TexasHoldem* pkr );
	bool assessFlopScenario( Player* bot ) override;
	bool assessTurnScenario( Player* bot ) override { return false; }  // TODO :: implement me
	bool assessRiverScenario( Player* bot ) override { return false; }  // TODO :: implement me

private:
	TexasHoldem* thGame;
};
//--------------------------------------------------------------------------------------------------
#endif /* END HEADERS_BRAIN_H */
