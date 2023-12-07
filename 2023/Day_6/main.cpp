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

// Advent of Code Day 6
// https://adventofcode.com/2023/day/5

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <complex>

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

auto countWins = [](const uint64_t t, const uint64_t d) {
    uint64_t wins{};
    for (uint64_t v = 0; v <= t; ++v) {
        if (v * (t-v) > d) {
            wins++;
        }
    }
    return wins;
};

auto winsAnalysis =[](const uint64_t t, const uint64_t d) {
    const auto s = sqrt(t * t / 4. - d);
    auto x1 = static_cast<uint64_t>(ceil(t / 2. - s));
    auto x2 = static_cast<uint64_t>(floor(t / 2. + s));
    x1 += (x1 * (t - x1) <= d) ? 1 : 0;
    x2 -= (x2 * (t - x2) <= d) ? 1 : 0;
    return x2 - x1 + 1;
};

auto toNumbers = [](string& line) {
    vector<uint64_t> numbers;
    istringstream iss(line);
    iss.ignore(9);

    uint64_t  x;
    while(iss >> x) {
        numbers.push_back(x);
    }

    return numbers;
};

auto toNumberFromDigits = [](string& line) {
    uint64_t number{};

    for (const auto c : line) {
        if (isdigit(c)) {
            number = number * 10 + (c - '0');
        }
    }

    return number;
};

int main() {

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    cout << "Day 6" << endl;

    {
        // PART 1
        const auto times = toNumbers(lines[0]);
        const auto distances = toNumbers(lines[1]);

        uint64_t p1{1};
        for (size_t i = 0; i < times.size(); i++) {
            p1 *=  winsAnalysis(times[i], distances[i]);
        }

        cout << "  Part 1" << endl;
        cout << "    Ways to beat the record : " << p1 << endl;

    }

    {  // Part 2
        uint64_t p2{1};
        p2 = winsAnalysis(toNumberFromDigits(lines[0]), toNumberFromDigits(lines[1]));
        cout << "  Part 2" << endl;
        cout << "    Ways to beat the record in one race : " << p2 << endl;
    }

    return EXIT_SUCCESS;
}
