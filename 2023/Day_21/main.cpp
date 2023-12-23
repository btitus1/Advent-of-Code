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

// Advent of Code Day 21
// https://adventofcode.com/2023/day/21

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cstdint>

#define PART1_STEPS 64
#define PART2_STEPS 26501365

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

using Pos = array<uint8_t, 2>;
using Dir = size_t;
using Grid = vector<string>;

struct State {
    explicit State(const Pos &pos, uint32_t step = 0, array<int16_t, 2> tile = array<int16_t, 2>{})
            : pos{pos}, step{step}, tile{tile} {
    }

    [[nodiscard]] uint64_t hash() const {
        uint64_t hash{};
        hash |= static_cast<uint64_t>(pos[0]);
        hash |= static_cast<uint64_t>(pos[1]) << 8;
        hash |= static_cast<uint64_t>(tile[0] + INT16_MAX) << 16;
        hash |= static_cast<uint64_t>(tile[1] + INT16_MAX) << 32;
        return hash;
    }

    Pos pos{};
    uint32_t step{};
    array<int16_t, 2> tile{};
};

static size_t bfs(const Grid &grid) {
    unordered_map<uint64_t, uint32_t> visited{};
    constexpr array<array<int8_t, 2>, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    queue<State> q;
    const Pos dim{static_cast<uint8_t>(grid.size()), static_cast<uint8_t>(grid[0].size())};
    for (uint8_t r = dim[0] / 2; r < dim[0]; ++r) {
        for (uint8_t c = dim[1] / 2; c < dim[1]; ++c) {
            if ('S' == grid[r][c]) {
                q.emplace(Pos{r, c});
                break;
            }
        }
    }
    size_t count{};
    while (!q.empty()) {
        const auto state = q.front();
        q.pop();
        const auto step = static_cast<uint32_t>(state.step + 1);
        if (step > PART1_STEPS) {
            continue;
        }
        const auto [r, c] = state.pos;
        for (Dir dir = 0; dir < adjs.size(); ++dir) {
            if (0 == dir && grid[0].size() - 1 == c) {  // E
                continue;
            }
            if (1 == dir && grid.size() - 1 == r) {  // S
                continue;
            }
            if (2 == dir && 0 == c) {  // W
                continue;
            }
            if (3 == dir && 0 == r) {  // N
                continue;
            }
            const auto &[rr, cc] = adjs[dir];
            const auto pos = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)};
            if ('#' == grid[pos[0]][pos[1]]) {
                continue;
            }
            const auto next = State{pos, step};
            const auto hash = next.hash();
            if (visited.find(hash) != visited.end()) {
                continue;
            }
            const auto rem = PART1_STEPS - next.step;
            if (0 == rem % 2) {
                count++;
            }
            visited[hash] = next.step;
            q.push(next);
        }
    }
    return count;
}

static int64_t bfsInf(const Grid &grid, uint32_t maxSteps) {
    unordered_map<uint64_t, uint32_t> visited{};
    constexpr array<array<int8_t, 2>, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    queue<State> q;
    const Pos dim{static_cast<uint8_t>(grid.size()), static_cast<uint8_t>(grid[0].size())};
    for (uint8_t r = dim[0] / 2; r < dim[0]; ++r) {
        for (uint8_t c = dim[1] / 2; c < dim[1]; ++c) {
            if ('S' == grid[r][c]) {
                q.emplace(Pos{r, c});
                break;
            }
        }
    }
    int64_t count{};
    while (!q.empty()) {
        const auto state = q.front();
        q.pop();
        const auto step = static_cast<uint32_t>(state.step + 1);
        if (step > maxSteps) {
            continue;
        }
        const auto [r, c] = state.pos;
        for (Dir dir = 0; dir < adjs.size(); ++dir) {
            Pos pos;
            auto tile = state.tile;
            if (0 == dir && grid[0].size() - 1 == c) {  // E
                pos = {r, 0};
                tile[0]++;
            } else if (1 == dir && grid.size() - 1 == r) {  // S
                pos = {0, c};
                tile[1]++;
            } else if (2 == dir && 0 == c) {  // W
                pos = {r, static_cast<uint8_t>(grid[0].size() - 1)};
                tile[0]--;
            } else if (3 == dir && 0 == r) {  // N
                pos = {static_cast<uint8_t>(grid.size() - 1), c};
                tile[1]--;
            } else {
                const auto &[rr, cc] = adjs[dir];
                pos = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)};
            }
            if ('#' == grid[pos[0]][pos[1]]) {
                continue;
            }
            const auto next = State{pos, step, tile};
            const auto hash = next.hash();
            if (visited.find(hash) != visited.end()) {
                continue;
            }
            const auto rem = maxSteps - next.step;
            if (0 == rem % 2) {
                count++;
            }
            visited[hash] = next.step;
            q.push(next);
        }
    }
    return count;
}

int64_t evalQuadratic(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, int64_t x) {
    return ((x - x2) * (x - x3) / ((x1 - x2) * (x1 - x3))) * y1 + ((x - x1) * (x - x3) / ((x2 - x1) * (x2 - x3))) * y2 +
           ((x - x1) * (x - x2) / ((x3 - x1) * (x3 - x2))) * y3;
}

int main() {
    vector<string> grid{};
    if (!readFile(file1, grid)) {
        return EXIT_FAILURE;
    }

    {
        // Part 1
        const auto amt = bfs(grid);
        cout << "  Part 1" << endl;
        cout << "     Plots reached in " << PART1_STEPS << " steps : " << amt << endl;
    }
    {  // Part 2
        constexpr uint64_t dim = 131;
        const auto amt = evalQuadratic(0, bfsInf(grid, (PART1_STEPS + 1) + 0 * dim), 1,
                                       bfsInf(grid, (PART1_STEPS + 1) + 1 * dim), 2,
                                       bfsInf(grid, (PART1_STEPS + 1) + 2 * dim),
                                       (PART2_STEPS - (PART1_STEPS + 1)) / dim);
        cout << "  Part 2" << endl;
        cout << "     Plots reached in " << PART2_STEPS << " steps : " << amt << endl;
    }

    return EXIT_SUCCESS;
}