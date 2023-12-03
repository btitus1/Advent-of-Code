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
#include <regex>

using namespace std;

const string file1 = "input1.txt";
const string file2 = "input2.txt";

vector <string> lines;

/*
 * Reads an input text file into a vector array of strings
 */
void readfile(const string& inputFile) {
	string line;
	ifstream file(inputFile);

	if (file.is_open()) {
		while (getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	} else {
     cout << "** Error: " << inputFile << " not found.";
    }
}

/*
 *  Finds the first digit in the line
 */
int findFirstDigit(const string& str) {
	for (int i = 0; i < (int) str.length(); i++) {
		if (isdigit(str[i])) {
			return (str[i] - '0');
		}
	}
	return -1;
}

/*
 * Finds the last digit in the line
 */
int findLastDigit(const string& str) {
	for (int i = (int) str.length(); i >= 0; i--) {
		if (isdigit(str[i])) {
			return (str[i] - '0');
		}
	}
	return -1;
}

int main()
{
	vector <string> newlines;

	string newline;

	int total = 0;
	int newTotal = 0;

	cout << "Day 1\n";

	// PART 1
    lines.clear();
    readfile(file1);

	for (int i = 0;i < lines.size();i++) {
		total += (findFirstDigit(lines[i]) * 10 + findLastDigit(lines[i]));
	}

	cout << "  Part 1\n" << "    Sum of all of the calibration values : " << total << "\n";

	// PART 2
    lines.clear();
    readfile(file2);

	for (int i = 0;i < lines.size();i++) {
		newline = regex_replace(lines[i], regex("zero"), "z0o");
		newline = regex_replace(newline, regex("one"), "o1e");
		newline = regex_replace(newline, regex("two"), "t2o");
		newline = regex_replace(newline, regex("three"), "t3e");
		newline = regex_replace(newline, regex("four"), "f4r");
		newline = regex_replace(newline, regex("five"), "f5e");
		newline = regex_replace(newline, regex("six"), "s6x");
		newline = regex_replace(newline, regex("seven"), "s7n");
		newline = regex_replace(newline, regex("eight"), "e8t");
		newline = regex_replace(newline, regex("nine"), "n9e");
		newlines.push_back(newline);
	}

	for (int i = 0;i < newlines.size();i++) {
		newTotal += (findFirstDigit(newlines[i]) * 10 + findLastDigit(newlines[i]));
	}

	cout << "  Part 2\n" << "    Sum of all of the calibration values : " << newTotal << "\n";
}
