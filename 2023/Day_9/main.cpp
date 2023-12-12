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

// Advent of Code Day 9
// https://adventofcode.com/2023/day/9

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <numeric>

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

int main() {

    cout << "Day 9" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }


    {
        // Part 1
        uint64_t sum {};
        for (const auto& line : lines) {
            vector<int> data;
            vector<int> lasts;

            int x;
            istringstream iss(line);
            while (iss >> x) {
                data.push_back(x);
            }

            lasts.push_back(data.back());
            do {
                adjacent_difference(data.begin(), data.end(), data.begin());
                lasts.push_back(data.back());
                data.erase(data.begin());

            } while (!all_of(data.begin(),data.end(), [](auto d) { return d == 0;}));

            for (const auto& last : lasts) {
                sum += last;
            }
        }

        cout << "  Part 1" << endl;
        cout << "     Sum of right extrapolated values : " << sum << endl;

    }

    {  // Part 2
        uint64_t sum {};

        for (const auto& line : lines) {
            vector<int> data;
            vector<int> firsts;

            int x;
            istringstream iss(line);
            while (iss >> x) {
                data.push_back(x);
            }

            do {
                firsts.push_back(data.front());
                adjacent_difference(data.begin(), data.end(), data.begin());

                data.erase(data.begin());

            } while (!all_of(data.begin(),data.end(), [](auto d) { return d == 0;}));

            int first{};
            for (size_t i = firsts.size(); i > 0; --i) {
                first = firsts[i -1] - first;
            }
            sum += first;
        }
        cout << "  Part 2" << endl;
        cout << "     Sum of left extrapolated values : " << sum << endl;
    }

    return EXIT_SUCCESS;
}
