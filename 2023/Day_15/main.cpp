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

// Advent of Code Day 15
// https://adventofcode.com/2023/day/15

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <deque>

using namespace std;

const string file1 = "input.txt";

class Step {
public:
    string label;
    char operation;
    int length;

    Step(string _label, char _operation, int _length) {
        label = std::move(_label);
        operation = _operation;
        length = _length;
    }
};

class Lens {
public:
    string label;
    int length;

    Lens(string _label, int _length) {
        label = std::move(_label);
        length = _length;
    }
};

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

int hash_alg(const string &step) {
    int cur_val = 0;
    for (char a: step) {
        cur_val += a;
        cur_val *= 17;
        cur_val %= 256;
    }

    return cur_val;
}

Step get_parts(const string &step) {
    stringstream s;
    char a;
    char op;
    int length = 0;
    string label;
    s << step;
    while (s >> a) {
        if (a == '=' || a == '-') {
            op = a;
        } else if (a >= '0' && a <= '9') {
            length = a - '0';
        } else {
            label += a;
        }
    }
    Step new_step(label, op, length);
    return new_step;
}

int main() {

    cout << "Day 15" << endl;

    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    const auto &line = lines[0];

    vector<string> steps;
    string temp;

    for (char a: line) {
        if (a != ',') {
            temp += a;
        } else {
            steps.push_back(temp);
            temp = "";
        }
    }

    temp += line[line.size() - 1];
    steps.push_back(temp);

    int total1 = 0;
    for (const string &step: steps) {
        total1 += hash_alg(step);
    }

    cout << "  Part 1" << endl;
    cout << "     Initialize sequence sum  : " << total1 << endl;

    int total2 = 0;
    deque<Lens> boxes[256];
    vector<Step> operations;
    operations.reserve(steps.size());

    for (const string &step: steps) {
        operations.push_back(get_parts(step));
    }

    for (auto &operation: operations) {
        int num = hash_alg(operation.label);

        if (operation.operation == '=') {
            bool found = false;
            for (auto &j: boxes[num]) {
                if (operation.label == j.label) {
                    j.length = operation.length;
                    found = true;
                    break;
                }
            }
            if (!found) {
                Lens new_lens(operation.label, operation.length);
                boxes[num].push_back(new_lens);
            }
        }
        if (operation.operation == '-') {
            for (int j = 0; j < boxes[num].size(); j++) {
                if (operation.label == boxes[num][j].label) {
                    boxes[num].erase(boxes[num].begin() + j);
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < boxes[i].size(); j++) {
            total2 += (i + 1) * (j + 1) * boxes[i][j].length;
        }
    }

    cout << "  Part 2" << endl;
    cout << "     Focusing power : " << total2 << endl;

    return 0;
}