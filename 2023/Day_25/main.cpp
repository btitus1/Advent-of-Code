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

// Advent of Code Day 25
// https://adventofcode.com/2023/day/25

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>

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

using Matrix = vector<pair<int, int>>;
using Names = map<string, int>;
using Edges = vector<vector<int>>;
Matrix m;  // adjacency matrix

Names names;    // nodes
Edges edges;    // edges

pair<int, vector<int>> globalMinCut(vector<vector<int>> mat)
{
    pair<int, vector<int>> best = {INT_MAX, {}};
    size_t n = mat.size();
    vector<vector<int>> co(n);

    for (int i = 0; i < n; i++)
        co[i] = {i};

    for (int ph = 1; ph < n; ph++)
    {
        vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++)
        { // O(V^2) -> O(E log V) with prio. queue
            w[t] = INT_MIN;
            s = t, t = max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++)
                w[i] += mat[t][i];
        }
        best = min(best, {w[t] - mat[t][t], co[t]});
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++)
            mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++)
            mat[i][s] = mat[s][i];
        mat[0][t] = INT_MIN;
    }

    return best;
}

auto split(const string& s, const string& delimiter) -> vector<string> {
    size_t pos_start = 0;
    size_t pos_end;
    size_t delim_len = delimiter.length();
    string token;
    vector<string> result;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        result.emplace_back(token);
    }

    result.emplace_back(s.substr(pos_start));
    return result;
}

auto getIndex(const string &name) {
    if (names.find(name) != names.end()) {
        return names[name];
    }
    int index = static_cast<int>(names.size());
    names[name] = index;
    return index;
}

Edges parse(vector<string> &lines) {
    for (const auto &line: lines) {
        vector<string> tokens = split(line, " ");
        tokens[0] = tokens[0].substr(0, tokens[0].size() - 1);
        int index = getIndex(tokens[0]);

        for (size_t i = 1; i < tokens.size(); ++i) {
            m.emplace_back(index, getIndex(tokens[i]));
        }
    }
    // Initializing a single row
    vector<int> row(names.size(), 0);
    // Initializing the 2-D vector
    Edges edg(names.size(), row);

    for (auto e: m) {
        edg[e.first][e.second] = 1;
        edg[e.second][e.first] = 1;
    }
    return edg;
}

int main() {
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    chrono::time_point<std::chrono::system_clock> startTime, endTime;

    {
        // Part 1
        cout << "  Part 1" << endl;
        startTime = chrono::system_clock::now();
        edges = parse(lines);
        pair<int, vector<int>> ret = globalMinCut(edges);
        size_t sol = ret.second.size() * (names.size() - ret.second.size());
        endTime = std::chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;

        cout << "     Two groups multiplied : " << sol << endl;
        cout << "     Elapsed time : " << elapsed_seconds.count() << "s" << endl;
    }
    return EXIT_SUCCESS;
}