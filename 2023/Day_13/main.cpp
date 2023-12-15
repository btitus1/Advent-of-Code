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

// Advent of Code Day 13
// https://adventofcode.com/2023/day/13

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

const string file1 = "sample.txt";

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


template <size_t TARGETDIFF = 0>
static size_t isvMirror(const std::vector<std::string>& map)
{
    size_t c, diff;
    for (c = 1; c < map[0].size(); ++c) {
        diff = 0;
        size_t mm{1}, cc{c};
        do {
            cc--;
            for (const auto & rr : map) {
                if (rr[cc] != rr[cc + mm]) {
                    if (++diff > TARGETDIFF) {
                        break;
                    }
                }
            }
            mm += 2;
        } while (cc > 0 && cc + mm <= map[0].size() && diff <= TARGETDIFF);
        if (TARGETDIFF == diff) {
            break;
        }
    }
    if (TARGETDIFF != diff) {
        return 0;
    }
    return c;
}

template <size_t TARGETDIFF = 0>
static size_t ishMirror(const std::vector<std::string>& map)
{
    size_t r, diff;
    for (r = 1; r < map.size(); ++r) {
        diff = 0;
        size_t mm{1}, rr{r};
        do {
            rr--;
            for (size_t cc = 0; cc < map[0].size(); ++cc) {
                if (map[rr][cc] != map[rr + mm][cc]) {
                    if (++diff > TARGETDIFF) {
                        break;
                    }
                }
            }
            mm += 2;
        } while (rr > 0 && rr + mm <= map.size() && diff <= TARGETDIFF);
        if (TARGETDIFF == diff) {
            break;
        }
    }
    if (TARGETDIFF != diff) {
        return 0;
    }
    return r;
}

template <size_t TARGETDIFF = 0>
static size_t pattern(size_t& s, const std::vector<std::string>& map)
{
    if (const auto r = ishMirror<TARGETDIFF>(map); r > 0) {
        s += 100 * r;
    } else {
        s += isvMirror<TARGETDIFF>(map);
    }
    return s;
}

int main()
{
    cout << "Day 13" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }
    const auto toMaps = [&lines]() {
        std::vector<std::vector<std::string> > maps;
        std::vector<std::string> map;
        lines.emplace_back();
        for (auto& line : lines) {
            if (line.empty() && !map.empty()) {
                maps.push_back(map);
                map.clear();
                continue;
            }
            map.push_back(std::move(line));
        }
        return maps;
    };
    const auto maps = toMaps();

    {
        // Part 1
        constexpr size_t targetDiff{0};
        const auto sum = accumulate(maps.cbegin(), maps.cend(), size_t{}, pattern<targetDiff>);
        cout << "  Part 1" << endl;
        cout << "     Sum : " << sum << endl;
    }
    {  // Part 2
        constexpr size_t targetDiff{1};
        const auto sum = accumulate(maps.cbegin(), maps.cend(), size_t{}, pattern<targetDiff>);
        cout << "  Part 2" << endl;
        cout << "     Sum : " << sum << endl;
    }

    return EXIT_SUCCESS;
}