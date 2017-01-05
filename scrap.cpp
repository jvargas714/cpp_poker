#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "logger.h"
#include "player.h"
using namespace std;


int main()
{
	log() << 1 << " " << 2 << " " << 3 << endl;
	Player p1;
	Player p2;
	Player p3;

	log() << p1 << 6969696 << 234234<<  p2 << " hellop " << p3 << "dah";

	return 0;
}