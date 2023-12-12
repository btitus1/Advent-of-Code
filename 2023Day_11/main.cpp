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

// Advent of Code Day 11
// https://adventofcode.com/2023/day/9

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

int main() {

    cout << "Day 11" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }
    { // Part 1

        cout << "  Part 1" << endl;
        cout << "     Value : " << true << endl;
    }

    {  // Part 2

        cout << "  Part 2" << endl;
        cout << "     Value : " << false << endl;
    }

    return EXIT_SUCCESS;
}
