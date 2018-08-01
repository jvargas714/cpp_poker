#include <iostream>
#include <set>
#include <ctime>
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

static void logWtime() {
    std::time_t tm = std::time(nullptr);
    std::cout << std::put_time( std::localtime( &tm ), "%c" ) << std::endl;
}

int main() {
    logWtime();
    return 0;
}

