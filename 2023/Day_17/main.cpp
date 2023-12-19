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

// Advent of Code Day 17
// https://adventofcode.com/2023/day/17

#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

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
using Dir = uint8_t;
using HeatMap = vector<string>;

struct State
{
    State(const Pos& pos, Dir dir, Dir step = 0, uint16_t heat = 0) : pos{pos}, dir{dir}, step{step}, heat{heat}
    {
    }
    Pos pos{};
    uint16_t heat{};
    Dir dir{};
    Dir step{};

    [[nodiscard]] uint32_t hash() const
    {
        uint32_t hash{};
        hash |= static_cast<uint32_t>(pos[0]);
        hash |= static_cast<uint32_t>(pos[1]) << 8;
        hash |= static_cast<uint32_t>(step) << 16;
        hash |= static_cast<uint32_t>(dir) << 24;
        return hash;
    }
};

static uint16_t dijkstra(const HeatMap& heatmap, const Pos& start, const Pos& end, Dir minStep = 1, Dir maxStep = 3)
{
    unordered_map<uint32_t, uint16_t> visited{};
    constexpr array<array<int8_t, 2>, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    auto cmp = [](const auto& a, const auto& b) { return a.heat > b.heat; };
    priority_queue<State, vector<State>, decltype(cmp)> q(cmp);
    for (Dir dir = 0 ; dir < static_cast<uint8_t>(adjs.size()); dir++) {
        q.emplace(start, dir);
    }
    while (!q.empty()) {
        const auto state = q.top();
        if (state.pos == end && state.step >= minStep) {
            return state.heat;
        }
        q.pop();
        const auto [r, c] = state.pos;
        for (Dir dir = 0; dir < static_cast<uint8_t >(adjs.size()); ++dir) {
            if (dir == (state.dir + 2) % adjs.size()) {  // must not reverse
                continue;
            }
            if (0 == dir && heatmap[0].size() - 1 == c) {  // E
                continue;
            }
            if (1 == dir && heatmap.size() - 1 == r) {  // S
                continue;
            }
            if (2 == dir && 0 == c) {  // W
                continue;
            }
            if (3 == dir && 0 == r) {  // N
                continue;
            }
            if (dir != state.dir && state.step < minStep) {  // must move at least min steps in same direction
                continue;
            }
            const Dir step = dir == state.dir ? state.step + 1 : 1;
            if (step > maxStep) {  // must not move more than max steps in same direction
                continue;
            }
            const auto& [rr, cc] = adjs[dir];
            const auto pos = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)}; // grab row and column
            const auto heat = static_cast<uint16_t>(state.heat + heatmap[pos[0]][pos[1]] - '0'); // convert heat to number
            const auto next = State{pos, dir, step, heat};
            const auto hash = next.hash();
            if (auto it = visited.find(hash); it != visited.end()) {
                if (heat < it->second) {
                    it->second = heat;
                    q.push(next);
                }
            } else {
                visited[hash] = heat;
                q.push(next);
            }
        }
    }
    return UINT16_MAX;
}
int main()
{
    cout << "Day 17" << endl;

    vector<string> heatmap{};
    if (!readFile(file1, heatmap)) {
        return EXIT_FAILURE;
    }

    const auto start = Pos{0, 0};
    const auto end = Pos{static_cast<uint8_t>(heatmap.size() - 1), static_cast<uint8_t>(heatmap[0].size() - 1)};

    {
        // Part 1
        const auto amt = dijkstra(heatmap, start, end, 1, 3);
        cout << "  Part 1" << endl;
        cout << "     Minimal heat loss (1 step min, 3 steps max): " << amt << endl;
    }
    {  // Part 2
        const auto amt = dijkstra(heatmap, start, end, 4, 10);
        cout << "  Part 2" << endl;
        cout << "     Minimal heat loss (4 steps min, 10 steps max): " << amt << endl;
    }

    return EXIT_SUCCESS;
}