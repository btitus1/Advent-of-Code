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

// Advent of Code Day 14
// https://adventofcode.com/2023/day/14

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

using namespace std;

const string file1 = "input.txt";

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

static void moveNorth(vector<string> &grid) {
    for (size_t r = 1; r < grid.size(); ++r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
            if (grid[r][c] == 'O') {
                size_t rr(r);
                size_t cc(c);
                while (rr > 0 && grid[rr - 1][cc] == '.') {
                    grid[rr - 1][cc] = 'O';
                    grid[rr][cc] = '.';
                    rr--;
                }
            }
        }
    }
}

static void moveWest(vector<string> &grid) {
    for (size_t c = 1; c < grid[0].size(); ++c) {
        for (size_t r = 0; r < grid.size(); ++r) {
            if (grid[r][c] == 'O') {
                size_t rr(r);
                size_t cc(c);
                while (cc > 0 && grid[rr][cc - 1] == '.') {
                    grid[rr][cc - 1] = 'O';
                    grid[rr][cc] = '.';
                    cc--;
                }
            }
        }
    }
}

static void moveSouth(vector<string> &grid) {
    for (size_t r = grid.size() - 1; r > 0; --r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
            if (grid[r - 1][c] == 'O') {
                size_t rr(r - 1);
                size_t cc(c);
                while (rr + 1 < grid.size() && grid[rr + 1][cc] == '.') {
                    grid[rr + 1][cc] = 'O';
                    grid[rr][cc] = '.';
                    rr++;
                }
            }
        }
    }
}

static void moveEast(vector<string> &grid) {
    for (size_t c = grid[0].size() - 1; c > 0; --c) {
        for (size_t r = 0; r < grid.size(); ++r) {
            if (grid[r][c - 1] == 'O') {
                size_t rr(r);
                size_t cc(c - 1);
                while (cc + 1 > 0 && grid[rr][cc + 1] == '.') {
                    grid[rr][cc + 1] = 'O';
                    grid[rr][cc] = '.';
                    cc++;
                }
            }
        }
    }
}

int main() {

    cout << "Day 14" << endl;

    vector<string> grid{};
    if (!readFile(file1, grid)) {
        return EXIT_FAILURE;
    }

    const auto load = [](const vector<string> &grid) {
        uint64_t sum{};
        for (size_t r = 0; r < grid.size(); ++r) {
            for (size_t c = 0; c < grid[r].size(); ++c) {
                if (grid[r][c] == 'O') {
                    sum += grid.size() - r;
                }
            }
        }
        return sum;
    };


    {
        // Part 1
        moveNorth(grid);

        const auto sum = load(grid);
        cout << "  Part 1" << endl;
        cout << "     Total Load : " << sum << endl;
    }
    {  // Part 2
        moveWest(grid);
        moveSouth(grid);
        moveEast(grid);

        size_t step{1};

        map<vector<string>, size_t> cache;
        cache.insert({grid, step});

        constexpr size_t stop(1000000000);
        while (step++ < stop) {
            moveNorth(grid);
            moveWest(grid);
            moveSouth(grid);
            moveEast(grid);

            if (auto it = cache.find(grid);  it != cache.end()) {
                const auto period = step - it->second;
                step = stop - (stop - step) % period;
                cache.clear();
            } else if (!cache.empty()) {
                cache.insert({grid, step});
            }
        }
        const auto sum = load(grid);
        cout << "  Part 2" << endl;
        cout << "     Total Load : " << sum << endl;
    }

    return EXIT_SUCCESS;
}