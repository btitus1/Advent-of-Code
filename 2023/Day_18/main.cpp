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

// Advent of Code Day 18
// https://adventofcode.com/2023/day/18

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

using namespace std;

const string file1 = "input.txt";

struct dig_t {
    char dir_1{};
    char dir_2{};
    int64_t dist1{};
    int64_t dist2{};
    string hexValue{};
};
struct pos_t {
    int64_t x, y;
};

pos_t operator+(const pos_t &a, const pos_t &b) { return {a.x + b.x, a.y + b.y}; }

pos_t operator*(const pos_t &a, int64_t b) { return {a.x * b, a.y * b}; }

using digs_t = vector<dig_t>;

int64_t shoelace(const vector<pos_t> &points) {
    int64_t left_sum = 0;
    int64_t right_sum = 0;

    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        left_sum += points[i].x * points[j].y;
        right_sum += points[j].x * points[i].y;
    }

    return abs(left_sum - right_sum) / 2;
}

size_t lava_capacity(const digs_t &digs, bool convert) {
    pos_t pos{0, 0};
    vector<pos_t> vertices;

    for (int i = 0; i < digs.size(); ++i) {
        auto &curr_dig = digs[i];
        auto &next_dig = digs[(i + 1) % digs.size()];

        char curr_dir = convert ? curr_dig.dir_1 : curr_dig.dir_2;
        char next_dir = convert ? next_dig.dir_1 : next_dig.dir_2;

        pos_t dir{};
        if (curr_dir == 'R') {
            dir = {1, 0};
        } else if (curr_dir == 'D') {
            dir = {0, 1};
        } else if (curr_dir == 'L') {
            dir = {-1, 0};
        } else if (curr_dir == 'U') {
            dir = {0, -1};
        }

        pos = pos + dir * (convert ? curr_dig.dist1 : curr_dig.dist2);

        if ((curr_dir == 'U' && next_dir == 'R') || (curr_dir == 'R' && next_dir == 'U')) {
            vertices.push_back(pos);
        } else if ((curr_dir == 'R' && next_dir == 'D') || (curr_dir == 'D' && next_dir == 'R')) {
            vertices.push_back(pos + pos_t{1, 0});
        } else if ((curr_dir == 'D' && next_dir == 'L') || (curr_dir == 'L' && next_dir == 'D')) {
            vertices.push_back(pos + pos_t{1, 1});
        } else if ((curr_dir == 'L' && next_dir == 'U') || (curr_dir == 'U' && next_dir == 'L')) {
            vertices.push_back(pos + pos_t{0, 1});
        }
    }

    return shoelace(vertices);
}

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

digs_t parse(const vector<string> &lines) {

    digs_t digs;
    for (const auto &line: lines) {
        dig_t dig;
        char dir;

        stringstream ss;
        ss << line;

        // part 1
        ss >> dig.dir_1 >> dig.dist1 >> dig.hexValue;
        dig.hexValue = dig.hexValue.substr(2, 6);

        // part 2
        dir = dig.hexValue[5];
        if (dir == '0') {
            dig.dir_2 = 'R';
        } else if (dir == '1') {
            dig.dir_2 = 'D';
        } else if (dir == '2') {
            dig.dir_2 = 'L';
        } else {
            dig.dir_2 = 'U';
        }
        stringstream(dig.hexValue.substr(0, 5)) >> hex >> dig.dist2;

        digs.push_back(dig);

    }

    return digs;
}

int main() {
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    // stores the parsed information
    digs_t digs = parse(lines);

    cout << "Day 18" << endl;
    {
        // Part 1
        size_t amt = lava_capacity(digs, true);
        cout << "  Part 1" << endl;
        cout << "     Cubic meters : " << amt << endl;
    }
    {  // Part 2
        size_t amt = lava_capacity(digs, false);
        cout << "  Part 2" << endl;
        cout << "     Corrected cubic meters : " << amt << endl;
    }

    return EXIT_SUCCESS;
}