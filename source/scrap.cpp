#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iostream>
using namespace std;

class Player {
public:
	Player(string name): name(name) {}
	std::string name;
	void namePlayer(const std::string& nm) { name = nm; }
};

Player* getPlayer(const std::string& name, std::vector<Player>& players)
{
	for(auto& player : players)
	{
		if ( player.name == "jerkoff" )
			return &player;
	}
	return nullptr;
}

int main()
{
	Player p1("asshole!");
	Player p2("fucktard");
	Player p3("nardnerdFuck");

	p3.namePlayer("jerkoff");
	std::vector<Player> plyrs = {p1, p2, p3};

    for(auto& el : plyrs)
        cout << el.name << endl;

	Player* plyr = getPlayer("jerkoff", plyrs);

	std::cout << "player that has been looked up is named: " << plyr->name << std::endl;
    plyr->name = "new jerkoff";

    for(auto& el : plyrs)
        cout << el.name << endl;

	return 0;
}