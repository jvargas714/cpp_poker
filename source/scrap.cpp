#include <utility>
#include <string>
#include <iostream> 
using namespace std; 


class Card {
public:
    int suite;
    int rk;
    std::string name;
public:
    Card()=default;
    Card(int rk, int st) : rk(rk), suite(st) {
        name = "testing";
    }
};

void showCard(const Card& cd) {
    cout << "Card: " << cd.suite << " " << cd.rk << " " << cd.name << endl;
}

int main() {
    std::pair<Card, Card> cards;
    showCard(cards.first);
    showCard(cards.second);
    return 0;
}

