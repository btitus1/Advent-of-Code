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

// Advent of Code Day 10
// https://adventofcode.com/2023/day/9

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <numeric>
#include <set>
#include <array>
#include <queue>
#include "conmanip.h"

using namespace std;
using Pos = array<int16_t, 2>;
using Pipes = set<pair<Pos, Pos>>;

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

// breadth first search for flood fill
size_t bfs(const Pipes &pipes, vector<string> &map) {
    const Pos dim{static_cast<int16_t> (map.size()), static_cast<int16_t> (map[0].size())};
    vector <vector<bool>> visited(map.size(), vector<bool>(map[0].size(), false));
    queue <Pos> q{};
    q.push({0, 0});
    const auto isVisited = [&](const Pos &pos) {
        return visited[pos[0]][pos[1]];
    };

    const auto isPipe = [&pipes](const Pos &&src, const Pos &&dst) {
        return pipes.find(std::pair(src, dst)) != pipes.end();
    };
    const auto updateQ = [&](const Pos &&next) {
        if (!isVisited(next)) {
            visited[next[0]][next[1]] = true;
            q.push(next);
        }
    };

    while (!q.empty()) {
        auto pos = q.front();
        q.pop();
        const auto [r, c] = pos;
        if (c < dim[1] - 1) {  // E
            if (!isPipe({static_cast<short>(r), static_cast<short>(c + 1)},
                        {static_cast<short>(r + 1), static_cast<short>(c + 1)})) {
                updateQ({static_cast<short>(r), static_cast<short>(c + 1)});
            }
        }
        if (r < dim[0] - 1) {  // S
            if (!isPipe({static_cast<short>(r + 1), c}, {static_cast<short>(r + 1), static_cast<short>(c + 1)})) {
                updateQ({static_cast<short>(r + 1), c});
            }
        }
        if (c > 0) {  // W
            if (!isPipe({r, c}, {static_cast<short>(r + 1), c})) {
                updateQ({r, static_cast<short>(c - 1)});
            }
        }
        if (r > 0) {  // N
            if (!isPipe({r, c}, {r, static_cast<short>(c + 1)})) {
                updateQ({static_cast<short>(r - 1), c});
            }
        }
    }
    using namespace conmanip;
    console_out_context ctxOut;
    console_out conOut(ctxOut);

    size_t count{};
    for (int16_t r = 0; r < dim[0] - 2; ++r) {
        for (int16_t c = 0; c < dim[1] - 2; ++c) {
            const auto isInside =
                    !isVisited({r, c}) && !isVisited({r, static_cast<short>(c + 1)}) &&
                    !isVisited({static_cast<short>(r + 1), c}) &&
                    !isVisited({static_cast<short>(r + 1), static_cast<short>(c + 1)});
            if (isInside) {
                count++;
            }
            // colorize
            if (isInside) {
                std::cout << settextcolor(console_text_colors::light_yellow) << map[r + 1][c + 1];
            } else if ('S' == map[r + 1][c + 1]) {
                std::cout << settextcolor(console_text_colors::light_green) << map[r + 1][c + 1];
            } else if (isPipe({static_cast<short>(r + 1), static_cast<short>(c + 1)},
                              {static_cast<short>(r + 2), static_cast<short>(c + 1)}) ||
                       isPipe({static_cast<short>(r + 1), static_cast<short>(c + 1)},
                              {static_cast<short>(r + 1), static_cast<short>(c + 2)}) ||
                       isPipe({r, static_cast<short>(c + 1)}, {static_cast<short>(r + 1), static_cast<short>(c + 1)}) ||
                       isPipe({static_cast<short>(r + 1), c}, {static_cast<short>(r + 1), static_cast<short>(c + 1)})) {
                std::cout << settextcolor(console_text_colors::light_red) << map[r + 1][c + 1];
            } else {
                std::cout << settextcolor(console_text_colors::blue) << map[r + 1][c + 1];
            }
        }
        // colorize
        std::cout << '\n';
    }

    ctxOut.restore(console_cleanup_options::restore_attibutes);
    return count;

}

int main() {

    cout << "Day 10" << endl;

    vector<string> map{};
    if (!readFile(file1, map)) {
        return EXIT_FAILURE;
    }

    const Pos dim{static_cast<int16_t> (map.size()), static_cast<int16_t> (map.begin()->length())};

    map.insert(map.begin(), string(dim[1], '.'));
    map.emplace_back(dim[1], '.');
    transform(map.begin(), map.end(), map.begin(), [](auto &row) { return '.' + std::move(row) + '.'; });

    const auto getStart = [&]() -> Pos {
        for (int16_t row = 1; row < dim[0] - 1; ++row) {
            for (int16_t col = 1; col < dim[1] - 1; ++col) {
                if ('S' == map[row][col]) {
                    return {row, col};
                }
            }
        }
        return {0, 0};
    };

    const auto start = getStart();
    Pipes pipes;

    { // Part 1
        const int16_t size = 4;
        constexpr array<Pos, size> adjs = {{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
        size_t max{};
        vector<int16_t> dirs;

        for (int16_t i = 0; i < static_cast<int16_t>(adjs.size()); ++i) {
            auto dir = i;
            if (find(dirs.begin(), dirs.end(), dir) != dirs.end()) {
                continue;
            }
            dirs.push_back(dir);
            Pos pos = {static_cast<int16_t>(start[0] + adjs[i][0]), static_cast<int16_t>(start[1] + adjs[i][1])};
            auto &[row, col] = pos;

            if (row < 0 || col < 0) {
                continue;
            }
            size_t steps{1};
            Pipes loop{};

            if (0 == dir || 1 == dir) {
                loop.insert({start, pos});
            } else {
                loop.insert({pos, start});
            }
            while (pos != start) {
                auto src = pos;
                if (0 == dir) {  // East
                    if ('-' == map[row][col]) {
                        col++;
                    } else if ('7' == map[row][col]) {
                        row++;
                        dir = 1;
                    } else if ('J' == map[row][col]) {
                        row--;
                        dir = 3;
                    } else {
                        steps = 0;
                        break;
                    }
                } else if (1 == dir) {  // South
                    if ('|' == map[row][col]) {
                        row++;
                    } else if ('J' == map[row][col]) {
                        col--;
                        dir = 2;
                    } else if ('L' == map[row][col]) {
                        col++;
                        dir = 0;
                    } else {
                        steps = 0;
                        break;
                    }
                } else if (2 == dir) { // West
                    if ('-' == map[row][col]) {
                        col--;
                    } else if ('L' == map[row][col]) {
                        row--;
                        dir = 3;
                    } else if ('F' == map[row][col]) {
                        row++;
                        dir = 1;
                    } else {
                        steps = 0;
                        break;
                    }
                } else if (3 == dir) { // North
                    if ('|' == map[row][col]) {
                        row--;
                    } else if ('F' == map[row][col]) {
                        col++;
                        dir = 0;
                    } else if ('7' == map[row][col]) {
                        col--;
                        dir = 2;
                    } else {
                        steps = 0;
                        break;
                    }
                }
                steps++;
                if (0 == dir || 1 == dir) {
                    loop.insert({src, pos});
                } else {
                    loop.insert({pos, src});
                }
                loop.insert({pos, src});
            }

            if (const auto steps2 = steps / 2; steps > 0) {

                const int16_t adder = 2;

                dirs.push_back(static_cast<int16_t>((dir + adder) % size));
                if (steps2 > max) {
                    max = steps2;
                    pipes = std::move(loop);
                }
            }
        }
        cout << "  Part 1" << endl;
        cout << "     Longest distance : " << max << endl;
    }

    {  // Part 2

        const auto count = bfs(pipes, map);
        cout << "  Part 2" << endl;
        cout << "     Enclosed tiles : " << count << endl;
    }

    return EXIT_SUCCESS;
}
