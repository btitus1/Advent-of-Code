#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <set>

using namespace std;

vector<uint64_t> allHands = {};
set<uint64_t> mySet = {};

vector<uint64_t> fourOfAKind = {};
vector<uint64_t> fullHouse = {};
vector<uint64_t> straight = {};
vector<uint64_t> threeOfAKind = {};

// Prime values for card ranks
const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };

// Hashing function using prime numbers for card ranks
uint64_t hashHand(const vector<int>& hand) {
    uint64_t hashValue = 1; // Initialize with 1 to avoid hashValue being 0
    for (int card : hand) {
        if (card >= 2 && card <= 14) {
            hashValue *= primes[card - 2];
        }
    }
    return hashValue;
}

// Function to generate Four of a Kind
void generateFourOfAKind() {

    // for all card types
    for (int j = 2; j <= 14; j++) {
        auto hand = vector<int>{};
        hand.reserve(5);

        // generate 4 cards
        for (int i = 0; i < 4; ++i) {
            hand.push_back(j);
        }
        // generate the 5th card
        for (int k = 2; k<= 14; ++k) {
            if (k!=j) {
                hand.push_back(k);
                uint64_t hashValue = hashHand(hand);
                fourOfAKind.push_back(hashValue);

                // all of them
                allHands.push_back(hashValue);
                mySet.insert(hashValue);

                hand.pop_back();
            }
        }
    }
}

// Function to generate Full House
void generateFullHouse() {

    // for all card types
    for (int j = 2; j <= 14; j++) {
        auto hand = vector<int>{};
        hand.reserve(5);

        // generate 3 cards
        for (int i = 0; i < 3; ++i) {
            hand.push_back(j);
        }
        // generate the other 2 cards
        for (int k = 2; k<= 14; ++k) {
            if (k!=j) {
                hand.push_back(k);
                hand.push_back(k);
                uint64_t hashValue = hashHand(hand);
                fullHouse.push_back(hashValue);

                // all of them
                allHands.push_back(hashValue);
                mySet.insert(hashValue);

                hand.pop_back();
                hand.pop_back();
            }
        }
    }
}

// Function to generate straights
void generateStraights() {

    // ace low straight
    vector<int> aceLow = {2,3,4,5,14};
    straight.push_back(hashHand(aceLow));

    // for all card types except 2
    for (int j = 2; j <= 10; j++) {
        vector<int> hand = {j,j+1,j+2,j+3,j+4};
        uint64_t hashValue = hashHand(hand);
        straight.push_back(hashValue);

        // all of them
        allHands.push_back(hashValue);
        mySet.insert(hashValue);
    }
}

// Function to generate trips
void generateTrips() {

    // for all card types
    for (int j = 2; j <= 14; j++) {
        auto hand = vector<int>{};
        hand.reserve(5);

        // generate 3 cards
        for (int i = 0; i < 3; ++i) {
            hand.push_back(j);
        }
        // generate the other 2 cards
        for (int k = 2; k<= 14; ++k) {
            for (int m = k+1; m<= 14; ++m) {
                if (k != j && m != j) {
                    hand.push_back(k);
                    hand.push_back(m);
                    uint64_t hashValue = hashHand(hand);
                    threeOfAKind.push_back(hashValue);

                    // all of them
                    allHands.push_back(hashValue);
                    mySet.insert(hashValue);

                    hand.pop_back();
                    hand.pop_back();
                }
            }
        }
    }
}

int main() {
    // Generate Four of a Kind
    generateFourOfAKind();
    cout << fourOfAKind.size() << " : Hash values of Four of a Kind " << endl;
    //for (uint64_t hv : fourOfAKind) {
    //    cout << hv << endl;
    //}

    // Generate Full House
    generateFullHouse();
    cout << fullHouse.size() << " : Hash values of Full House " << endl;
    //for (uint64_t hv : fullHouse) {
    //    cout << hv << endl;
    //}

    // Generate straight
    generateStraights();
    cout <<  straight.size() << " : Hash values of Straight " << endl;
    //for (uint64_t hv : straights) {
    //    cout << hv << endl;
    //}

    // Generate 3 of a kind
    generateTrips();
    cout <<  threeOfAKind.size() << " : Hash values of Trips " << endl;
    //for (uint64_t hv : threeOfAKind) {
    //    cout << hv << endl;
    //}

    // Summary
    cout << "size of All hands : " << allHands.size() << endl;
    cout << "size of my Set : " << mySet.size() << endl;

    return 0;
}
