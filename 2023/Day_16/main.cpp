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

// Advent of Code Day 16
// https://adventofcode.com/2023/day/16

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

using Coord = uint8_t;
using Pos = array<Coord, 2>;
using Dir = uint8_t;
using Beam = pair<Pos, Dir>;

struct BeamHash {
    size_t operator()(const Beam &beam) const {
        return (beam.first[0] << 8) + beam.first[1];
    }
};

const string file1 = "input.txt";

static bool reflect(Beam &beam, const vector<string> &grid) {
    auto &[pos, dir] = beam;
    const auto &[r, c] = pos;

    if (0 == dir) { // right
        if ('\\' == grid[r][c]) {
            dir = 1;
        } else if ('/' == grid[r][c]) {
            dir = 3;
        } else if ('|' == grid[r][c]) {
            dir = 1;
            return true;
        }
    } else if (1 == dir) { // down
        if ('\\' == grid[r][c]) {
            dir = 0;
        } else if ('/' == grid[r][c]) {
            dir = 2;
        } else if ('-' == grid[r][c]) {
            dir = 0;
            return true;
        }
    } else if (2 == dir) { // left
        if ('\\' == grid[r][c]) {
            dir = 3;
        } else if ('/' == grid[r][c]) {
            dir = 1;
        } else if ('|' == grid[r][c]) {
            dir = 1;
            return true;
        }
    } else if (3 == dir) { // up
        if ('\\' == grid[r][c]) {
            dir = 2;
        } else if ('/' == grid[r][c]) {
            dir = 0;
        } else if ('-' == grid[r][c]) {
            dir = 0;
            return true;
        }
    }
    return false;
}


static bool move(Beam &beam, const vector<string> &grid) {
    auto &[pos, dir] = beam;
    auto &[r, c] = pos;

    if (0 == dir) { // right
        if (grid[0].size() - 1 == c) {
            return false;
        }
        c++;

    } else if (1 == dir) { // down
        if (grid.size() - 1 == r) {
            return false;
        }
        r++;
    } else if (2 == dir) { // left
        if (0 == c) {
            return false;
        }
        c--;
    } else if (3 == dir) { // up
        if (0 == r) {
            return false;
        }
        r--;
    }
    return true;
}

static uint64_t simulate(const Beam &start, vector<string> &grid) {
    queue<Beam> beams;
    beams.push(start);
    unordered_set<Beam, BeamHash> visited;

    while (!beams.empty()) {
        auto beam = beams.front();
        beams.pop();

        while (visited.find(beam) == visited.end()) {
            visited.insert(beam);
            if (reflect(beam, grid)) {
                const auto &[pos, dir] = beam;
                beams.emplace(pos, (dir + 2) % 4);
            }

            if (!move(beam, grid)) {
                break;
            }
        }
    }

    unordered_set<uint64_t> energized;
    energized.reserve(visited.size());
    transform(visited.cbegin(), visited.cend(), inserter(energized, energized.end()),
              [](const Beam &beam) { return BeamHash{}(beam); });
    return energized.size();
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

    cout << "Day 16" << endl;

    vector<string> grid{};
    if (!readFile(file1, grid)) {
        return EXIT_FAILURE;
    }

    {
        // Part 1
        uint64_t max = simulate({{0, 0}, 0}, grid);
        cout << "  Part 1" << endl;
        cout << "     Energized tiles : " << max << endl;
    }
    {  // Part 2
        uint64_t maxTiles{};
        for (Coord r = 1; r < grid.size(); ++r) {
            maxTiles = max(maxTiles, simulate({{r, 0}, 0}, grid));
            maxTiles = max(maxTiles, simulate({{r, static_cast<Coord>(grid[0].size() - 1)}, 2}, grid));
        }
        for (Coord c = 1; c < grid.size(); ++c) {
            maxTiles = max(maxTiles, simulate({{0, c}, 1}, grid));
            maxTiles = max(maxTiles, simulate({{static_cast<Coord>(grid[0].size() - 1),c}, 3}, grid));
        }

        cout << "  Part 2" << endl;
        cout << "     Max Energized tiles: " << maxTiles << endl;
    }

    return EXIT_SUCCESS;
}