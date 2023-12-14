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

// Advent of Code Day 12
// https://adventofcode.com/2023/day/12

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

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

using Group = vector<uint32_t>;
using Puzzle = pair<string, Group>;
using Puzzles = vector<Puzzle>;

template <size_t FOLD = 1, bool WITH_CACHE = true>
static size_t count(const string& p, const Group& g, size_t pos = 0, size_t k = 0,
                    shared_ptr<map<pair<size_t, size_t>, size_t> > cache = nullptr)
{
    if constexpr (WITH_CACHE) {
        assert(nullptr != cache);
        if (const auto it = cache->find({pos, k}); it != cache->end()) {
            return it->second;
        }
    }
    if (pos == p.size() && k == FOLD * g.size()) {
        return 1;
    }
    if (pos >= p.size()) {
        return 0;
    }
    if ('#' == p[pos]) {
        if (k >= FOLD * g.size() || pos + g[k % g.size()] >= p.size()) {
            return 0;
        }
        for (size_t c = 1; c < g[k % g.size()]; ++c) {
            if ('.' == p[pos + c]) {
                return 0;
            }
        }
        pos += g[k % g.size()];
        k++;
        if ('#' == p[pos]) {
            return 0;
        }
        const auto s = count<FOLD, WITH_CACHE>(p, g, pos + 1, k, cache);
        if constexpr (WITH_CACHE) {
            cache->insert({{pos + 1, k}, s});
        }
        return s;
    }
    if ('.' == p[pos]) {
        return count<FOLD, WITH_CACHE>(p, g, pos + 1, k, cache);
    }
    const string p1 = p.substr(0, pos) + '#' + p.substr(pos + 1, p.size() - pos - 1);
    const string p2 = p.substr(0, pos) + '.' + p.substr(pos + 1, p.size() - pos - 1);
    return count<FOLD, WITH_CACHE>(p1, g, pos, k, cache) + count<FOLD, WITH_CACHE>(p2, g, pos, k, cache);
}

static inline size_t count1(const string& p, const Group& g)
{
    return count<1, false>(p, g);
}

static inline size_t count5(const string& p, const Group& g)
{
    return count<5>(p, g, 0, 0, make_shared<map<pair<size_t, size_t>, size_t> >());
}

int main() {
    cout << "Day 12" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    {  // Part 1
        Puzzles puzzles;
        for (const auto& line : lines) {
            uint32_t x;
            string p;
            istringstream iss{line};
            iss >> p;
            p += '.';
            Group g;
            while (iss >> x) {
                g.push_back(x);
                iss.ignore(1);
            }
            puzzles.emplace_back(p, g);
        }

        size_t sum{};
        for (const auto& [p, g] : puzzles) {
            sum += count1(p, g);
        }
        cout << "  Part 1" << endl;
        cout << "     Sum of possible arrangements : " << sum << endl;
    }
    {  // Part 2
        Puzzles puzzles;
        for (const auto& line : lines) {
            uint32_t x;
            string p;
            istringstream iss{line};
            iss >> p;
            p = p + '?' + p + '?' + p + '?' + p + '?' + p + '.';
            Group g;
            while (iss >> x) {
                g.push_back(x);
                iss.ignore(1);
            }
            puzzles.emplace_back(p, g);
        }

        size_t sum{};
        for (const auto& [p, g] : puzzles) {
            sum += count5(p, g);
        }
        cout << "  Part 2" << endl;
        cout << "     Sum of possible arrangements : " << sum << endl;
    }

    return EXIT_SUCCESS;
}