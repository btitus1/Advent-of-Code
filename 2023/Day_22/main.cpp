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

// Advent of Code Day 22
// https://adventofcode.com/2023/day/22

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

using namespace std;

using Cube = array<uint16_t, 3>;
using Brick = array<Cube, 2>;
using Bricks = vector<Brick>;
using IndexSet = unordered_set<uint16_t>;
const string file1 = "input.txt";

static bool overlap(const Brick &a, const Brick &b) {
    return (max(a[0][0], b[0][0]) < min(a[1][0], b[1][0])) &&
           (max(a[0][1], b[0][1]) < min(a[1][1], b[1][1]));
}


struct Jenga {
    Jenga(const Bricks &_bricks, uint16_t minZ) : bricks{_bricks} {
        // Move to the ground
        for (auto &b: bricks) {
            b[0][2] -= minZ;
            b[1][2] -= minZ;
        }

        for (uint16_t i = 0; i < bricks.size(); ++i) {
            for (uint16_t j = i + 1; j < bricks.size(); ++j) {
                const auto &a = bricks[i];
                const auto &b = bricks[j];

                if (!overlap(a, b)) {
                    continue;
                }
                if (a[1][2] <= b[0][2]) {
                    above[i].insert(j);
                    below[j].insert(i);
                } else if (b[1][2] <= a[0][2]) {
                    above[j].insert(i);
                    below[i].insert(j);
                }
            }
        }
        settle();
    }

    void settle() {
        bool moved;
        do {
            moved = false;
            for (uint16_t i = 0; i < bricks.size(); ++i) {
                auto &b = bricks[i];
                if (auto it = below.find(i); it != below.end()) {
                    int16_t minZ(UINT16_MAX);
                    for (auto j: it->second) {
                        const auto &a = bricks[j];
                        minZ = min<uint16_t>(minZ, b[0][2] - a[1][2]);
                        if (0 == minZ) {
                            break;
                        }
                    }
                    if (minZ > 0) {
                        b[1][2] -= minZ;
                        b[0][2] -= minZ;
                        moved = true;
                    }
                } else if (0 != b[0][2]) {
                    b[1][2] -= b[0][2];
                    b[0][2] = 0;
                    moved = true;
                }
            }
        } while (moved);
    }

    size_t settle(uint16_t k) const {
        auto bricks_copy = bricks;
        bricks_copy[k][1][2] = bricks_copy[k][0][2];
        IndexSet movers;

        bool moved;
        do {
            moved = false;
            for (uint16_t i = 0; i < bricks.size(); ++i) {
                auto &b = bricks_copy[i];
                if (b[0][2] < bricks_copy[k][1][2]) {
                    continue;
                }
                if (auto it = below.find(i); it != below.end()) {
                    int16_t minZ(UINT16_MAX);
                    for (auto j: it->second) {
                        const auto &a = bricks_copy[j];
                        minZ = min<uint16_t>(minZ, b[0][2] - a[1][2]);
                        if (0 == minZ) {
                            break;
                        }
                    }
                    if (minZ > 0) {
                        b[1][2] -= minZ;
                        b[0][2] -= minZ;
                        moved = true;
                        movers.insert(i);
                    }
                } else if (0 != b[0][2]) {
                    b[1][2] -= b[0][2];
                    b[0][2] = 0;
                    moved = true;
                    movers.insert(i);
                }
            }
        } while (moved);
        return movers.size();
    }

    bool willMove(uint16_t k) const {
        const auto &c = bricks[k];
        if (auto jt = above.find(k); jt != above.end()) {
            for (auto j: jt->second) {
                const auto &b = bricks[j];
                if (c[1][2] < b[0][2]) {
                    continue;
                }
                if (auto it = below.find(j); it != below.end()) {
                    uint16_t minZ{UINT16_MAX};
                    for (auto i: it->second) {
                        if (i == k) {
                            continue;
                        }
                        const auto &a = bricks[i];
                        minZ = min<uint16_t>(minZ, b[0][2] - a[1][2]);
                    }

                    if (minZ > 0) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    IndexSet disintegrable() const {
        IndexSet removable;
        for (uint16_t i = 0; i < bricks.size(); ++i) {
            if (!willMove(i)) {
                removable.insert(i);
            }
        }
        return removable;
    }

    size_t countFalling() const
    {
        size_t count{};
        for (uint16_t i=0; i < bricks.size(); ++i) {
            count +=settle(i);
        }
        return count;
    }

private:
    Bricks bricks;
    unordered_map<uint16_t, IndexSet> above;
    unordered_map<uint16_t, IndexSet> below;
};

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
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    const auto toBricks = [](const auto &lines) -> pair<Bricks, uint64_t> {
        Bricks bricks{};
        uint16_t minZ(UINT16_MAX);
        for (const auto &line: lines) {
            istringstream iss(line);
            Brick b;
            for (uint16_t i = 0; i < 2; ++i) {
                for (uint16_t j = 0; j < 3; ++j) {
                    uint16_t v;
                    iss >> v;
                    iss.ignore(1);
                    b[i][j] = v + i;
                }
                minZ = min(minZ, b[i][2]);
            }
            bricks.emplace_back(b);
        }
        return {bricks, minZ};
    };

    const auto &[bricks, minZ] = toBricks(lines);
    const auto jenga = Jenga(bricks, minZ);

    {
        // Part 1
        cout << "  Part 1" << endl;
        cout << "     Blocks that can be disintegrated : " << jenga.disintegrable().size() << endl;
    }
    {  // Part 2
        cout << "  Part 2" << endl;
        cout << "     Sum of other bricks that can fall : " << jenga.countFalling() << endl;
    }

    return EXIT_SUCCESS;
}