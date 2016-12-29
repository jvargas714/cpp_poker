#include "test_suite.h"

int main( int argc, char* argv[] )
{
	poker_tester tester;
	tester.test_bot_ctors();
	tester.test_bot_chen_hand( TEST_CNT );
	tester.test_hand_detection( TEST_CNT );
	return 0;
}
