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

// Advent of Code Day 23
// https://adventofcode.com/2023/day/23

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <chrono>

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
using Grid = vector<string>;
constexpr array<array<int8_t, 2>, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

struct State {
    explicit State(const Pos &pos) : pos{pos} {
    }

    State(const Pos &pos, const set<Pos> &_path) : pos{pos}, path{_path} {
        path.insert(pos);
    }

    Pos pos{0, 0};
    set<Pos> path{};
    size_t dist{};
    Pos prev{0, 0};
    bool branched{false};
};

struct Graph {
    Pos root{};
    set<Pos> nodes;
    map<Pos, set<pair<Pos, size_t> > > edges;
};

static size_t longest_path(const Grid &grid, const Pos &start, const Pos &end) {
    const auto isNode = [&](const Pos &pos) {
        if ((pos[0] == start[0] && pos[1] == start[1]) || (pos[0] == end[0] && pos[1] == end[1])) {
            return true;
        }
        const auto [r, c] = pos;
        size_t count{};
        for (size_t dir = 0; dir < adjs.size(); ++dir) {
            const auto [rr, cc] = adjs[dir];
            const auto adj = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)};
            if ('#' != grid[adj[0]][adj[1]]) {
                count++;
            }
        }
        return count > 2 && '#' != grid[r][c];
    };

    map<pair<Pos, uint8_t>, size_t> visited;
    visited[{start, 1}] = 0;
    queue<State> q{};
    auto first = State{start};
    first.prev = start;
    q.push(first);
    Graph g;
    g.root = start;
    const auto updateQ = [&](const State &next, uint8_t dir) {
        if (auto it = visited.find({next.pos, dir}); it != visited.end()) {
            if (next.branched || isNode(next.pos)) {
                q.push(next);
            }
        } else {
            visited[{next.pos, dir}] = next.path.size();
            q.push(next);
        }
    };

    while (!q.empty()) {
        auto state = q.front();
        q.pop();
        size_t dist{};
        const auto branched = isNode(state.pos);
        if (branched) {
            g.nodes.insert(state.pos);
            dist = state.path.size();
            if (dist > state.dist) {
                g.edges[state.prev].insert({state.pos, dist - state.dist});
                g.edges[state.pos].insert({state.prev, dist - state.dist});
            }
        }
        const auto [r, c] = state.pos;
        for (size_t dir = 0; dir < adjs.size(); ++dir) {
            if (1 == dir && r == end[0] && c == end[1]) {  // S
                continue;
            }
            if (3 == dir && r == start[0] && c == start[1]) {  // N
                continue;
            }
            const auto [rr, cc] = adjs[dir];
            const auto pos = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)};
            if ('#' == grid[pos[0]][pos[1]]) {
                continue;
            }
            if (state.path.find(pos) != state.path.end()) {
                continue;
            }
            auto next = State{pos};
            next.path = state.path;
            next.path.insert(state.pos);
            next.branched = branched;
            if (dist > 0) {
                next.prev = state.pos;
                next.dist = dist;
            } else {
                next.prev = state.prev;
                next.dist = state.dist;
            }
            updateQ(next, dir);
        }
    }

    auto cmp = [](const auto &a, const auto &b) { return a.dist < b.dist; };
    priority_queue<State, vector<State>, decltype(cmp)> q2(cmp);
    visited.clear();
    q2.emplace(g.root);
    visited[{g.root, 0}] = 0;
    const auto updateQ2 = [&](const State &next) {
        if (auto it = visited.find({next.pos, 0}); it != visited.end()) {
            if (next.dist > it->second) {
                it->second = next.dist;
            }
            q2.push(next);
        } else {
            visited[{next.pos, 0}] = next.dist;
            q2.push(next);
        }
    };
    while (!q2.empty()) {
        const auto state = q2.top();
        q2.pop();
        const auto &dsts = g.edges.at(state.pos);
        for (const auto &[pos, w]: dsts) {
            if (state.path.find(pos) != state.path.end()) {
                continue;
            }
            auto next = State{pos};
            next.path = state.path;
            next.path.insert(state.pos);
            next.dist = state.dist + w;
            updateQ2(next);
        }
    }
    return visited.at({end, 0});
}

static size_t dijkstra_dag(const Grid &grid, const Pos &start, const Pos &end) {
    map<Pos, size_t> visited;
    visited[start] = 0;
    auto cmp = [](const auto &a, const auto &b) { return a.path.size() < b.path.size(); };
    priority_queue<State, vector<State>, decltype(cmp)> q(cmp);
    auto first = State{start};
    q.push(first);
    const auto updateQ = [&](const State &next) {
        if (auto it = visited.find(next.pos); it != visited.end()) {
            if (next.path.size() > it->second) {
                it->second = next.path.size();
                q.push(next);
            }
        } else {
            visited[next.pos] = next.path.size();
            q.push(next);
        }
    };

    while (!q.empty()) {
        const auto state = q.top();
        q.pop();
        const auto [r, c] = state.pos;
        for (size_t dir = 0; dir < adjs.size(); ++dir) {
            if (dir != 0 && '>' == grid[r][c]) {  // E
                continue;
            }
            if (dir != 1 && 'v' == grid[r][c]) {  // S
                continue;
            }
            if (dir != 2 && '<' == grid[r][c]) {  // W
                continue;
            }
            if (dir != 3 && '^' == grid[r][c]) {  // N
                continue;
            }
            if (1 == dir && r == end[0] && c == end[1]) {  // S
                continue;
            }
            if (3 == dir && r == start[0] && c == start[1]) {  // N
                continue;
            }
            const auto [rr, cc] = adjs[dir];
            const auto pos = Pos{static_cast<uint8_t>(r + rr), static_cast<uint8_t>(c + cc)};
            if ('#' == grid[pos[0]][pos[1]]) {
                continue;
            }
            if (state.path.find(pos) != state.path.end()) {
                continue;
            }
            updateQ(State{pos, state.path});
        }
    }

    return visited.at(end);
}

int main() {
    vector<string> grid{};
    if (!readFile(file1, grid)) {
        return EXIT_FAILURE;
    }

    const auto start = Pos{0, 1};
    const auto end = Pos{static_cast<uint8_t>(grid.size() - 1), static_cast<uint8_t>(grid[0].size() - 2)};

    chrono::time_point<std::chrono::system_clock> startTime, endTime;

    {
        // Part 1
        startTime = chrono::system_clock::now();
        const auto steps = dijkstra_dag(grid, start, end);
        endTime = std::chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;

        cout << "  Part 1" << endl;
        cout << "     Steps in Longest Hike : " << steps << endl;
        cout << "     Elapsed time : " << elapsed_seconds.count() << "s" << endl;
    }
    {  // Part 2
        startTime = chrono::system_clock::now();
        const auto steps = longest_path(grid, start, end);
        endTime = std::chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;
        cout << "  Part 2" << endl;
        cout << "     Steps in Longest Hike with no slips: " << steps << endl;
        cout << "     Elapsed time : " << elapsed_seconds.count() << "s" << endl;
    }
    return EXIT_SUCCESS;
}