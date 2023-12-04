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

// Advent of Code Day 2
// https://adventofcode.com/2023/day/2


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string file1 = "input1.txt";

const int REDMAX = 12;
const int GREENMAX = 13;
const int BLUEMAX = 14;

vector<string> explode(char d, const string& s) {
    vector<string> v;
    string buf;
    for (char i : s)
    {
        if (i == d) {
            v.push_back(move(buf));
            buf.clear();
        } else {
            buf.push_back(i);
        }
    }
    v.push_back(move(buf));
    if (v.back().empty()) {
        v.pop_back();
    }
    return v;
}

int main() {
    cout << "Day 2\n";

    ifstream inputFile(file1);

    if (inputFile.is_open()) {
        string line;
        int gameTotal = 0;
        int gameNumber = 1;
        int gamePower = 0;

        while (getline(inputFile, line)) {
            line = line.substr(line.find(':') + 2);

            auto games = explode(';', line);

            bool gamePossible = true;
            int redMin = 0;
            int greenMin = 0;
            int blueMin = 0;

            for (const auto& game : games) {
                auto cubes = explode(',', game);

                for (auto& cube : cubes) {
                    // eliminate leading space
                    if (!cube.empty() && cube.front() == ' ') {
                        cube.erase(cube.begin());
                    }

                    // grab the number as a string and convert to integer
                    int count = stoi(cube.substr(0, cube.find(' ')));

                    // check limits for each color
                    auto color = cube.substr(cube.find(' ') + 1);
                    if (color == "red") {
                        if (count > REDMAX) {
                            gamePossible = false;
                        }
                        if (count > redMin) {
                            redMin = count;
                        }
                    } else if (color == "green") {
                        if (count > GREENMAX) {
                            gamePossible = false;
                        }
                        if (count > greenMin) {
                            greenMin = count;
                        }
                    } else if (color == "blue") {
                        if (count > BLUEMAX) {
                            gamePossible = false;
                        }
                        if (count > blueMin) {
                            blueMin = count;
                        }
                    }
                }
            }

            if (gamePossible) {
                gameTotal += gameNumber;
            }
            gamePower += (redMin * blueMin * greenMin);
            gameNumber++;
        }

        cout << "  Total : " << gameTotal << endl;
        cout << "  Power : " << gamePower << endl;
    }
}
