#include "poker_types.h"
#include "assessment.h"
#include "hand_weight.h"
#include <algorithm>

const std::vector<std::vector<int>> HAND_INDEX_COMBINATIONS = {
{0, 1, 2, 3, 4},
{0, 1, 2, 3, 5},
{0, 1, 2, 3, 6},
{0, 1, 2, 5, 4},
{0, 1, 2, 6, 4},
{0, 1, 5, 3, 4},
{0, 1, 6, 3, 4},
{0, 5, 2, 3, 4},
{0, 6, 2, 3, 4},
{5, 1, 2, 3, 4},
{6, 1, 2, 3, 4},
{5, 6, 2, 3, 4},
{5, 1, 6, 3, 4},
{5, 1, 2, 6, 4},
{5, 1, 2, 3, 6},
{0, 5, 6, 3, 4},
{0, 5, 2, 6, 4},
{0, 5, 2, 3, 6},
{0, 1, 5, 6, 4},
{0, 1, 5, 3, 6},
{0, 1, 2, 5, 6}
};


std::vector<std::string> assessment::getHandSuits(const Cards &cds) {
    std::vector<std::string> suits;
    for (const Card &cd: cds)
        suits.push_back(cd.getSuit());
    return suits;
}

std::vector<int> assessment::getRankInd(const Cards &cds) {
    std::vector<int> rankInds(cds.size());
    int n = 0;
    for (auto& el : cds)
        rankInds[n++] = el.rankIndex;
    return rankInds;
}

std::vector<int> assessment::dupRankInd(const Cards &cds) {
    std::vector<int> handRanks = assessment::getRankInd(cds);
    std::vector<int> dupRanksInd;
    int cnt = 0;
    int tmp = 0;
    int index = 0;

    for (int i = 0; i < 13; i++) {
        for (const auto rk : handRanks) {
            if (rk == i) {
                cnt++;
                if (cnt == 1) {
                    tmp = index;
                } else if (cnt == 2) {
                    dupRanksInd.push_back(tmp);
                    dupRanksInd.push_back(index);
                } else if (cnt > 2) {
                    dupRanksInd.push_back(index);
                }
            }
            index++;
        }
        index = 0;
        cnt = 0;
        tmp = 0;
    }
    return dupRanksInd;
}

bool assessment::hasPair(const Cards &cds) {
    std::vector<int> dupRanks = assessment::dupRankInd(cds);
    bool condition = false;
    if (dupRanks.size() == 2) {
        condition = true;
    }
    return condition;
}

bool assessment::hasTwoPair(const Cards &cds) {
    std::vector<int> handInd = assessment::dupRankInd(cds);
    std::vector<int> handRanks = assessment::getRankInd(cds);
    // checks to see if we have 4 in handInd and its not a 4 of a kind
    bool condition = handInd.size() == 4 &&
            std::count(handRanks.begin(), handRanks.end(), cds[handInd[0]].rankIndex) != 4;
    return condition;
}

bool assessment::hasTrips(const Cards &cds) {
    std::vector<int> dupInd = assessment::dupRankInd(cds);
    return dupInd.size() == 3;
}

bool assessment::hasFourOfAKind(const Cards &cds) {
    using std::count;
    std::vector<int> dupInd = assessment::dupRankInd(cds);
    if (dupInd.size() == 4) {
        Card sampleCard = cds[dupInd[0]];
        return std::count(cds.begin(), cds.end(), sampleCard) == 4;
    } else {
        return false;
    }
}

bool assessment::hasStraight(const Cards &cds) {
    std::vector<std::string> suits = assessment::getHandSuits(cds);
    Cards cards = cds;
    std::sort(cards.begin(), cards.end());
    int cnt = 0;
    for (uint32_t i = 1; i < cards.size(); i++) {
        if (cards[i].rankIndex - cards[i - 1].rankIndex == 1) {
            cnt++;
        }
    }
    return cnt == 4 && std::count(suits.begin(), suits.end(), suits[0]) != 5;
}

bool assessment::hasFlush(const Cards &cds) {
    using std::count;

    std::vector<std::string> suits = assessment::getHandSuits(cds);
    Cards cards = cds;
    std::sort(cards.begin(), cards.end());
    int cnt = 0;
    for (int i = 1; i < 5; i++) {
        if (cards[i].rankIndex - cards[i - 1].rankIndex == 1) {
            cnt++;
        }
    }
    // checking for a flush only and not a straight flush
    return std::count(suits.begin(), suits.end(), suits[0]) == 5 && cnt != 4
           && !assessment::hasFullHouse(cds);
}

bool assessment::hasFullHouse(const Cards &cds) {
    Cards cards = cds;

    if (cards.size() != 5) {
        throw poker_error::HandIDError("Error: @hasFullHouse -->hand size not 5 cards");
    }

    std::sort(cards.begin(), cards.end());
    int a = (int) std::count(cards.begin(), cards.end(), cards[0]);        // either 2, 3, or 0
    int b = (int) std::count(cards.begin(), cards.end(), cards[4]);        // either 2, 3, or 0
    return (a + b) == 5;      // if a pair and trips in one hand then we have a fullHouse
}

bool assessment::hasStraightFlush(const Cards &cds) {
    Cards cards = cds;
    std::vector<std::string> suits = assessment::getHandSuits(cds);
    std::sort(cards.begin(), cards.begin());
    int rnkCnt = 0, stCnt = 0;
    for (int i = 1; i < 5; i++) {
        if (cards[i].rankIndex - cards[i - 1].rankIndex == 1) {
            rnkCnt++;
        }
    }
    stCnt = (int) std::count(suits.begin(), suits.end(), suits[0]);
    return stCnt == 5 && rnkCnt == 4 && cards[4].getRank() != "A";
}

bool assessment::hasRoyalFlush(const Cards &cds) {
    Cards cards = cds;
    std::vector<std::string> suits = assessment::getHandSuits(cds);
    std::sort(cards.begin(), cards.end());
    int rnkCnt = 0, stCnt = 0;

    for (int i = 1; i < 5; i++) {
        if (cards[i].rankIndex - cards[i - 1].rankIndex == 1) {
            rnkCnt++;
        }
    }
    stCnt = (int) std::count(suits.begin(), suits.end(), suits[0]);
    return stCnt == 5 && rnkCnt == 4 && cards[4].getRank() == "A";
}

Cards assessment::getPair(const Cards &cds) {
    Cards pair;
    if (!hasPair(cds)) {
        throw poker_error::HandIDError("Error--> assessment::getPair(): HandIDError");
    } else {
        std::vector<int> rankIndex = dupRankInd(cds);
        pair.push_back(cds[rankIndex[0]]);
        pair.push_back(cds[rankIndex[1]]);
    }
    return pair;
}

Cards assessment::getTwoPair(const Cards &cds) {
    Cards cards;
    if (!hasTwoPair(cds)) {
        throw (poker_error::HandIDError("HandIDError-->assessment::getTwoPair()"));
    } else {
        std::vector<int> dupInd = dupRankInd(cds);
        auto i = dupInd.begin();

        while (i != dupInd.end()) {
            cards.push_back(cds[*i]);
            i++;
        }
    }
    std::sort(cards.begin(), cards.end());
    return cards;
}

Cards assessment::getTrips(const Cards &cds) {
    Cards cards;
    if (!hasTrips(cds)) {
        throw poker_error::HandIDError("HandIDError-->assessment::getTrips()");
    } else {
        std::vector<int> dupInd = dupRankInd(cds);
        auto i = dupInd.begin();
        while (i != dupInd.end()) {
            cards.push_back(cds[*i]);
            i++;
        }
    }
    return cards;
}

Cards assessment::getFourOfAKind(const Cards &cds) {
    std::vector<int> dupInd = dupRankInd(cds);
    Cards cards;

    if (hasFourOfAKind(cds)) {
        auto i = dupInd.begin();
        while (i != dupInd.end()) {
            cards.push_back(cds[*i]);
            i++;
        }
    } else
        throw poker_error::HandIDError("HandIDError-->assessment::getFourOfAKind()");
    return cards;
}

Cards assessment::getStraight(const Cards &cds) {
    if (!hasStraight(cds)) {
        throw poker_error::HandIDError("HandIDError-->assessment::getStraight()");
    } else {
        // copy bc cannot change hand in player object do to constness
        Cards local_hand(cds);
        std::sort(local_hand.begin(), local_hand.end());
        return local_hand;
    }
}

Cards assessment::getFlush(const Cards &cds) {
    if (!hasFlush(cds)) {
        throw poker_error::HandIDError("HandIDError-->assessment::getFlush()");
    } else {
        // copy due to constness
        Cards local_hand(cds);
        std::sort(local_hand.begin(), local_hand.end());
        return local_hand;
    }
}

Cards assessment::getFullHouse(const Cards &cds) {
    if (!hasFullHouse(cds)) {
        throw poker_error::HandIDError("Error: HandIDError --> "
                                       "@assessment::getFullHouse(), player does not have a full house!");
    } else {
        // copy due to constness
        Cards local_hand(cds);
        std::sort(local_hand.begin(), local_hand.end());
        return local_hand;
    }
}

Cards assessment::getStraightFlush(const Cards &cds) {
    if (!hasStraightFlush(cds)) {
        throw poker_error::HandIDError("Error: @assessment::getStraightFlush()-->HandIDError--> "
                                       "player does not have a Straight Flush");
    } else {
        return cds;
    }
}

Cards assessment::getRoyalFlush(const Cards &cds) {
    if (!hasRoyalFlush(cds)) {
        throw poker_error::HandIDError("HandIDError: @assessment::getRoyalFlush -->"
                                       " Player does not have royal flush");
    } else {
        return cds;
    }
}

HAND assessment::findHandStrength(const Cards &cards) {
    // runs through all assessment functions to determine a hand strength and return it
    using std::count;
    HAND hand;
    Cards cds = cards;
    std::sort(cds.begin(), cds.end());
    hand.hand_str = "error";

    if (hasRoyalFlush(cds)) {
        hand.strength = HandMapper::handMap.at("royalFlush")[0];
        hand.hand_str = "a Royal Flush";
        hand.cards = getRoyalFlush(cds);
        hand.type = ROYAL_FLUSH;
    } else if (hasStraightFlush(cds)) {
        int highCardRank = cds[4].rankIndex;
        hand.strength = HandMapper::handMap.at("straightFlush")[highCardRank];
        hand.hand_str = "a Straight Flush";
        hand.cards = getStraightFlush(cds);
        hand.type = STRAIGHT_FLUSH;
    } else if (hasFourOfAKind(cds)) {
        Cards inHand = getFourOfAKind(cds);
        hand.strength = HandMapper::handMap.at("fourOfAKind")[inHand[inHand.size()-1].rankIndex];
        hand.hand_str = "a Four of a Kind";
        hand.cards = getFourOfAKind(cds);
        hand.type = FOUR_OF_A_KIND;
    } else if (hasFullHouse(cds)) {
        Cards fullHouse = getFullHouse(cds);
        int cardRnk;
        if (std::count(fullHouse.begin(), fullHouse.end(), fullHouse[0]) == 3)
            cardRnk = fullHouse[0].rankIndex;
        else
            cardRnk = fullHouse[4].rankIndex;
        hand.strength = HandMapper::handMap.at("fullHouse")[cardRnk];
        hand.hand_str = "a Full House";
        hand.cards = fullHouse;
        hand.type = FULL_HOUSE;
    } else if (hasFlush(cds)) {
        Cards flush = getFlush(cds);
        int cardRnk = flush[4].rankIndex;
        hand.strength = HandMapper::handMap.at("flush")[cardRnk];
        hand.hand_str = "a Flush";
        hand.cards = flush;
        hand.type = FLUSH;
    } else if (hasStraight(cds)) {
        Cards straight = getStraight(cds);
        int cardRnk = straight[4].rankIndex;
        hand.strength = HandMapper::handMap.at("straight")[cardRnk];
        hand.hand_str = "a Straight";
        hand.cards = straight;
        hand.type = STRAIGHT;
    } else if (hasTrips(cds)) {
        Cards trips = getTrips(cds);
        int cardRnk = trips[0].rankIndex;
        hand.strength = HandMapper::handMap.at("trips")[cardRnk];
        hand.hand_str = "a Three of a Kind";
        hand.cards = trips;
        hand.type = TRIPS;
    } else if (hasTwoPair(cds)) {
        Cards twoPair = getTwoPair(cds);
        int cardRnk = twoPair[3].rankIndex;
        hand.strength = HandMapper::handMap.at("twoPair")[cardRnk];
        hand.hand_str = "Two Pair";
        hand.cards = twoPair;
        hand.type = TWO_PAIR;
    } else if (hasPair(cds)) {
        Cards pair = getPair(cds);
        int cardRnk = pair[0].rankIndex;
        hand.strength = HandMapper::handMap.at("pair")[cardRnk];
        hand.hand_str = "a Pair";
        hand.cards = pair;
        hand.type = PAIR;
    } else {
        std::sort(cds.begin(), cds.end());
        int cardRnk = cds[4].rankIndex;
        hand.strength = HandMapper::handMap.at("high")[cardRnk];
        hand.hand_str = "a High Card";
        hand.cards.push_back(cds[4]);
        hand.type = HIGH_CARD;

    }
    LOG << "hand type: " << hand.hand_str << ", with a hand strength of " << hand.strength << END;

    return hand;
}

std::ostream &operator<<(const Hand& hand, std::ostream &os) {
    os << "Hand:\n\tstrength: " << hand.strength;
    os << "\n\thand Type: " << assessment::handTypeToString(hand.type);
    os << "\n\thand str:" << hand.hand_str;
    os << "\n\t" << "cards: ";
    for (const auto& cd: hand.cards)
        os << "\n\t\t" << cd;
    return os;
}

std::string assessment::handTypeToString(const HAND_TYPE& type) {
    switch (type) {
        case HAND_TYPE::NONE: return "NONE";
        case HAND_TYPE::HIGH_CARD: return "HIGH CARD";
        case HAND_TYPE::PAIR: return "PAIR";
        case HAND_TYPE::TWO_PAIR: return "TWO PAIR";
        case HAND_TYPE::TRIPS: return "THREE OF A KIND";
        case HAND_TYPE::STRAIGHT: return "STRAIGHT";
        case HAND_TYPE::FLUSH: return "FLUSH";
        case HAND_TYPE::STRAIGHT_FLUSH: return "STRAIGHT FLUSH";
        case HAND_TYPE::FOUR_OF_A_KIND: return "FOUR OF A KIND";
        case HAND_TYPE::ROYAL_FLUSH: return "ROYAL FLUSH";
        default:
            return "UNKNOWN";
    }
}