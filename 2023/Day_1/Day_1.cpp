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

// Advent of Code Day 1
// https://adventofcode.com/2023/day/1


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstring>

using namespace std;

const string file1 = "input1.txt";
const string file2 = "input2.txt";

vector<string> lines;

/*
 * Reads an input text file into a vector array of strings
 */
void readfile(const string& inputFile, vector<string>& strLines) {
    string line;
    ifstream file(inputFile);

    if (file.is_open()) {
        while (getline(file, line)) {
            strLines.push_back(line);
        }
        file.close();
    } else {
        cout << "** Error: " << inputFile << " not found.";
    }
}

/*
 * Finds the first digit in the line
 */
int findFirstDigit(const string& str) {
    for (char ch : str) {
        if (isdigit(ch)) {
            return ch - '0';
        }
    }
    return -1;
}

/*
 * Finds the last digit in the line
 */
int findLastDigit(const string& str) {
    for (int i = static_cast<int>(str.length()) - 1; i >= 0; i--) {
        if (isdigit(str[i])) {
            return str[i] - '0';
        }
    }
    return -1;
}

int main() {
    int total = 0;
    int newTotal = 0;

    cout << "Day 1\n";

    // PART 1
    lines.clear();
    readfile(file1, lines);

    for (const auto& line : lines) {
        total += findFirstDigit(line) * 10 + findLastDigit(line);
    }

    cout << "  Part 1\n"
         << "    Sum of all calibration values: " << total << "\n";

    // PART 2
    vector<string> newlines;
    lines.clear();
    readfile(file2, lines);

    for (const auto& line : lines) {
        string newline = line;
        for (const auto& replacement : {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}) {
            size_t pos = newline.find(replacement);
            while (pos != string::npos) {
                newline.replace(pos, strlen(replacement), to_string(pos));
                pos = newline.find(replacement, pos + 1);
            }
        }
        newlines.push_back(newline);
    }

    for (const auto& newline : newlines) {
        newTotal += findFirstDigit(newline) * 10 + findLastDigit(newline);
    }

    cout << "  Part 2\n"
         << "    Sum of all calibration values: " << newTotal << "\n";

    return 0;
}