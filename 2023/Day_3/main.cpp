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

// Advent of Code Day 3
// https://adventofcode.com/2023/day/3


#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <array>

using namespace std;

const string file1 = "input.txt";

constexpr char dot{'.'};
constexpr char gear{'*'};

using Pos = array<unsigned int,2>;
typedef map<Pos, char> SymbolMap;
typedef map<Pos, vector<unsigned int>> NumberMap;

static bool readFile(const string& fileName, vector<string>& lines) {
   ifstream in(fileName);
   if (!in) {
       cerr << "Cannot open file " << fileName << endl;
       return false;
   }

   auto closeStream =  [&in] {
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
    vector<string> map;

    if (!readFile(file1, map)) {
        return EXIT_FAILURE;
    }

    Pos dim{static_cast<unsigned int>(map.size()), static_cast<unsigned int>(map[0].size())};
    map.insert(map.begin(), string(dim[1],dot));

    for (auto& row : map) {
        row = dot + row + dot;
    }

    SymbolMap symbols;
    for(unsigned int i = 1; i < dim[0] + 1; i++) {
        for (unsigned int j = 1; j < dim[1] + 1; j++) {
            if (!isdigit(map[i][j]) && dot != map[i][j]) {
                symbols[Pos{i,j}] = map[i][j];
            }
        }
    }

    NumberMap numbers;
    for(unsigned int i = 1; i < dim[0] + 1; i++) {
        vector<char> number;
        vector<Pos> adjs;

        for (unsigned int j=1; j < dim[1] + 2; j++) {
            if (isdigit(map[i][j])) {
                if (number.empty()) {
                    adjs.push_back(Pos{i-1,j-1});
                    adjs.push_back(Pos{i,j-1});
                    adjs.push_back(Pos{i+1,j-1});
                }
                number.push_back(map[i][j]);
                adjs.push_back(Pos{i-1, j});
                adjs.push_back(Pos{i+1, j});
            } else {
                if (!number.empty()) {
                    adjs.push_back(Pos{i-1,j});
                    adjs.push_back(Pos{i,j});
                    adjs.push_back(Pos{i+1,j});

                    unsigned int n = stoull(string(number.begin(), number.end()));

                    for (const auto& adj : adjs) {
                        numbers[adj].push_back(n);
                        }
                    }
                    number.clear();
                    adjs.clear();
                }

            }
        }
    cout << "Day 3\n";

    // PART 1
    unsigned int sumPart1 = 0;
    for(const auto& symbol : symbols) {
        const auto& pos = symbol.first;
        const auto& it = numbers.find(pos);

        if (it != numbers.end()) {
            for (const auto& value : it->second) {
                sumPart1 += value;
            }
        }
    }
    cout << "  Part 1\n"
         << "    Sum of all part numbers : " << sumPart1 << "\n";

    // PART 2
    unsigned int sumPart2 = 0;
    for(const auto& symbol : symbols) {
        const auto& pos = symbol.first;
        const auto& sym = symbol.second;

        if (gear == sym) {
            const auto& it = numbers.find(pos);
            if (it != numbers.end() && it->second.size() == 2) {
                sumPart2 += it->second[0] * it->second[1];

            }
        }
    }

    cout << "  Part 2\n"
         << "    Sum of all gear ratios : " << sumPart2 << "\n";
}
