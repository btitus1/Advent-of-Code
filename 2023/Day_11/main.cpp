/*
 * Copyright © 2023 Brian Titus
 * Contact: realcookiemonster69@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files(the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// Advent of Code Day 11
// https://adventofcode.com/2023/day/11

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

const string file1 = "input.txt";

using Pos = pair<uint16_t, uint16_t>;

int64_t manhattanDistance(const Pos &a, const Pos &b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

static bool readFile(const string &fileName, vector<string> &lines) {
    ifstream in(fileName);
    if (!in) {
        cerr << "Cannot open file " << fileName << endl;
        return false;
    }

    auto closeStream = [&in] {
        in.close();
    };

    string str;
    while (getline(in, str)) {
        lines.push_back(str);
    }

    closeStream();
    return true;
}

int main() {

    cout << "Day 11" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    vector<size_t> rowExpansions;
    for (size_t r = 0; r < lines.size(); ++r) {
        bool shouldExpand = true;
        for (char c : lines[r]) {
            if (c == '#') {
                shouldExpand = false;
                break;
            }
        }
        if (shouldExpand) {
            rowExpansions.push_back(r);
        }
    }
    vector<size_t> colExpansions;
    for (size_t c = 0; c < lines[0].size(); ++c) {
        bool shouldExpand = true;
        for (auto & line : lines) {
            if (line[c] == '#') {
                shouldExpand = false;
                break;
            }
        }
        if (shouldExpand) {
            colExpansions.push_back(c);
        }
    }

    vector<Pos> positions;
    for (size_t r = 0; r < lines.size(); ++r) {
        for (size_t c = 0; c < lines[r].size(); ++c) {
            if (lines[r][c] == '#') {
                positions.emplace_back(r, c);
            }
        }
    }

    {  // Part 1
        uint64_t sum{};
        for (size_t i = 0; i < positions.size(); ++i) {
            for (size_t j = i + 1; j < positions.size(); ++j) {
                sum += manhattanDistance(positions[i], positions[j]);
                for (const auto r: rowExpansions) {
                    if ((positions[i].first < r && r < positions[j].first) || (positions[j].first < r && r < positions[i].first)) {
                        sum += 2 - 1;
                    }
                }
                for (const auto c: colExpansions) {
                    if ((positions[i].second < c && c < positions[j].second) || (positions[j].second < c && c < positions[i].second)) {
                        sum += 2 - 1;
                    }
                }
            }
        }
        cout << sum << endl;
    }
    {  // Part 2
        uint64_t sum{};
        for (size_t i = 0; i < positions.size(); ++i) {
            for (size_t j = i + 1; j < positions.size(); ++j) {
                sum += manhattanDistance(positions[i], positions[j]);
                for (const auto r: rowExpansions) {
                    if ((positions[i].first < r && r < positions[j].first) || (positions[j].first < r && r < positions[i].first)) {
                        sum += 1000000 - 1;
                    }
                }
                for (const auto c: colExpansions) {
                    if ((positions[i].second < c && c < positions[j].second) || (positions[j].second < c && c < positions[i].second)) {
                        sum += 1000000 - 1;
                    }
                }
            }
        }
        cout << sum << endl;
    }

    return EXIT_SUCCESS;
}