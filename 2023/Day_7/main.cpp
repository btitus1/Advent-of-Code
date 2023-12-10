// Advent of Code 2023
// Day 7: Camel Cards
// https://adventofcode.com/2023/day/7

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <sstream>
#include <unordered_map>

const std::string lookup = "lookup.txt";
const std::string file1 = "input.txt";

// Prime values for card ranks
const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };
const std::vector<char> cardValues = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};

// Hashing function using prime numbers for card ranks
uint64_t hashHand(const std::string& hand) {
    uint64_t hashValue = 1; // Initialize with 1 to avoid hashValue being 0
    for (const auto& card : hand) {

        // get card index
        auto index = find(cardValues.begin(), cardValues.end(),card) - cardValues.begin();
        hashValue *= primes[index];

        }
    return hashValue;
    }


static bool readFile(const std::string &fileName, std::vector<std::string> &lines) {
    std::ifstream in(fileName);
    if (!in) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return false;
    }

    auto closeStream = [&in] {
        in.close();
    };

    std::string str;
    while (getline(in, str)) {
        lines.push_back(str);
    }

    closeStream();
    return true;
}

int main() {

    // read the lookup into map
    std::vector<std::string> lines{};
    if (!readFile(lookup, lines)) {
        return EXIT_FAILURE;
    }

    // map = hashcode, rank
    std::unordered_map<std::uint64_t, uint32_t> handLookup = {};
    for (const auto &line: lines) {
        std::istringstream iss{line};

        uint32_t rank;
        std::string hand;
        uint64_t hashcode;

        iss >> rank >> hand >> hashcode;
        handLookup[hashcode] = rank;
    }

    // read the text file
    lines.clear();
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    std::vector<std::pair<std::string, uint64_t>> hands;
    for (const auto & line : lines) {
        std::istringstream iss{line};
        std::string hand;
        uint64_t bid;
        iss >> hand >> bid;
        hands.emplace_back(hand, bid);
    }


    // Vector pairs to store ranks, hand, and bids
    std::vector<std::tuple<uint64_t, std::string,uint64_t>> tuples;

    // lookup the hand and create new tuples
    for (auto & handPair : hands) {
        std::string hand = handPair.first;
        uint64_t bid = handPair.second;

        uint64_t hashcode = hashHand(hand);
        auto got = handLookup.find(hashcode);
        if (got != handLookup.end()) {
            tuples.emplace_back(got->second, hand, bid);
        }
        else {
            std::cout << "Couldn't find hashcode : " << hand << ":" << hashcode << std::endl;
        }
    }

    sort(tuples.begin(), tuples.end());

    uint64_t total = 0;
    int index = 1;
    for (auto [ X, Y, Z ] : tuples) {
        uint64_t value = Z * index;
        std::cout << index << " : " << Y << " " << Z << " : " << Z * index << std::endl;
        total += value;
        index++;
    }
    std::cout << "total : " << total << std::endl;
}