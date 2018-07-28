#include <iostream>
#include <set>

#include "card.h"
#include "poker_types.h"
using namespace std;

typedef std::vector<Card> Cards;
typedef std::vector<std::vector<int>> Matrix;

void showCard(const Card& cd) {
    cout << cd << endl;
}

std::set<Cards>  getCardPermutes(Cards& cards) {
    Cards tmp;
    std::set<Cards> result;
    std::vector<Cards> groups;
    do {
        groups.push_back(cards);
    } while ( std::next_permutation(cards.begin(), cards.end()) );

    for (auto& grp : groups) {
        std::sort(grp.begin(), grp.end());
        result.insert(grp);
    }
    cout << "size of permutes: " << result.size() << endl;
    return result;
}

// 0 1 2 3 4 5 6
std::vector<Cards> combinationCards(const Cards& cards, int n) {
    std::vector<Cards> result;

}

// 0 1 2 3 4 5 6
// replace none
// 0 1 2 3 4

// replace 1
// 0 1 2 3 5
// 0 1 2 3 6

// 0 1 2 5 4
// 0 1 2 6 4

// 0 1 5 3 4
// 0 1 6 3 4

// 0 5 2 3 4
// 0 6 2 3 4

// 5 1 2 3 4
// 6 1 2 3 4

// replace 2
// 5 6 2 3 4  // i = 0, j = 1

// 5 1 6 3 4   // i = 0, j = 2
// 5 1 2 6 4  // i = 0, j = 3
// 5 1 2 3 6

// 0 5 6 3 4
// 0 5 2 6 4
// 0 5 2 3 6

// 0 1 5 6 4
// 0 1 5 3 6

// 0 1 2 5 6
// TODO :: FINISH THIS
Matrix selfPermute(const std::vector<int>& nums, uint32_t choose) {
    if (nums.empty() || choose > nums.size()) return {{}};
    auto len = static_cast<uint32_t>(nums.size());
    if (choose == len) return {nums};

    Matrix result;
    int replaceCnt = len - choose;  // max number of elements to be replaced
    int cnt = 1;
    return {{}};
}


int main() {
    std::vector<Card> cards = { Card(TWO, HEART), Card(THREE, CLUB),
                                Card(FOUR, SPADE), Card(FIVE, CLUB), Card(SIX, DIAMOND)};

    std::set<Cards> perms = getCardPermutes(cards);

    // display
    for (const auto& grp : perms) {
        for (const auto& cd : grp)
            cout << cd << endl;
        cout << "=========================" << endl;
    }
}

