#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <string>
#include <vector>

/*
index range: 0-12   0    1    2	   3	4    5    6    7    8     9    1-   11   12
string rank[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

index range 0-3    0    1    2    3
string suit[4] = {"C", "D", "H", "S"};
*/

class Card {
private:
    const static std::string suit[4];
    const static std::string rank[13];

protected:
    std::string rk, st;                // actual rank (string type)

public:
    int rankIndex, suitIndex;        // index in actual array (int type)
    Card();

    Card(int rkInd, int stInd);

    ~Card();

    const std::string getRank() const;    // returns string representation of ranking
    const std::string getSuit() const;    // returns string representation of suite
    friend std::ostream &operator<<(std::ostream &os, const Card &cd);

    // op overload cards are tested according to their ranking
    bool operator==(const Card &cd) const;

    bool operator<(const Card &cd) const;

    bool operator>(const Card &cd) const;

    // compare function of cards returns 0 if equal -1 if less than and 1 if greater than
    int cmp(const Card &cd) const;
};


// functor to set comparator for STL containers 
struct CardCmp {
    bool operator()(Card cd1, Card cd2) const;
};

/*
NOTES:
	for a set of cards the default comparison function is the result of the 
	operator <. In a set an object is checked for equality if the following
	expression returns true: !(a < b) && !(b < a) , if elements a and b are 
	considered to placed into the set. 
	
	A functor can be created to override the () operator which then can be 
	passed as the comparison object when constructing the set object. set<Card, cmp>

*/

#endif // CARD_H_INCLUDED
