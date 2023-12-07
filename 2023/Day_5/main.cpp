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

// Advent of Code Day 5
// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

const string file1 = "input.txt";

using Range = array<uint64_t,3>;
using Map = vector<Range>;
using Maps = vector<Map>;

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

    cout << "Day 5" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    Maps maps;
    {
        lines.push_back("");
        Map map;
        for (size_t i = 3; i < lines.size(); ++i) {
            if (lines[i].empty()) {
                maps.push_back(map);
                map.clear();
                i++;
                continue;
            }
            std::istringstream iss{lines[i]};
            uint64_t d, s, l;
            iss >> d >> s >> l;
            map.push_back({d, s, l});
        }
    }

    {
        // Part 1
        std::vector<uint64_t> seeds;
        std::istringstream iss{lines[0]};
        iss.ignore(6);
        uint64_t x;
        while (iss >> x) {
            seeds.push_back(x);
        }

        auto values = seeds;
        for (const auto& map : maps) {
            for (auto& v : values) {
                for (const auto& range : map) {
                    if (v >= range[1] && v < range[1] + range[2]) {
                        v += range[0] - range[1];
                        break;
                    }
                }
            }
        }
        const auto minLocation = *std::min_element(values.begin(), values.end());

        cout << "  Part 1" << endl;
        cout << "     Lowest location : " << minLocation << endl;

    }

    {  // Part 2
        std::vector<std::array<uint64_t, 2>> seeds;
        std::istringstream iss{lines[0]};
        iss.ignore(6);
        uint64_t x, y;
        while (iss >> x >> y) {
            seeds.push_back({x, y});
        }

        uint64_t minLocation{UINT64_MAX};
        for (const auto& seed : seeds) {
            for (uint64_t j = 0; j < seed[1]; ++j) {
                uint64_t v = seed[0] + j;
                std::vector<uint64_t> toSkip;
                for (auto & map : maps) {
                    uint64_t minSkip{UINT64_MAX};
                    for (const auto& range : map) {
                        if (v >= range[1]) {
                            if (v < range[1] + range[2]) {
                                toSkip.push_back(range[1] + range[2] - v - 1);
                                v += range[0] - range[1];
                                break;
                            }
                        } else {
                            minSkip = std::min(minSkip, range[1] - v - 1);
                        }
                    }
                    if (minSkip < UINT64_MAX) {
                        toSkip.push_back(minSkip);
                    }
                }
                if (!toSkip.empty()) {
                    j += *std::min_element(toSkip.begin(), toSkip.end());
                }
                minLocation = std::min(minLocation, v);
            }
        }

        cout << "  Part 2" << endl;
        cout << "     Lowest location of initial seeds: " << minLocation << endl;
    }

    return EXIT_SUCCESS;
}
