#include "test_suite.h"

// global logger
#ifdef	TESTING
logger* logger::inst = &logger::instance( "test_suite.log" );
#else
logger* logger::inst = &logger::instanace( "poker.log" );
#endif

int main( int argc, char* argv[] )
{
	test_bot_ctors();
	test_bot_chen_hand( TEST_CNT );
	return 0;
}
