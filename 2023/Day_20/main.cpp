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

// Advent of Code Day 20
// https://adventofcode.com/2023/day/20

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <sstream>
#include <array>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

enum class ModuleType {
    Undefined,
    Broadcast,
    FlipFlop,
    Conjunction
};

struct Module {
    uint64_t cycle{};
    map<string, uint64_t> cycles;
    bool state{false};
    map<string, bool> inputs;
    vector<string> outputs;
    ModuleType type{ModuleType::Undefined};
};

using Count = array<uint64_t, 2>;
using Graph = map<string, Module>;
using Pulse = tuple<string, string, bool>;

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

static Count simulate(Graph &g, uint64_t step) {

    Count count{1, 0};
    queue<Pulse> q;
    {
        const auto &broadcaster = g.at("broadcaster");
        count[0] += broadcaster.outputs.size();
        for (const auto &next: broadcaster.outputs) {
            q.emplace("broadcaster", next, false);
        }
    }
    while (!q.empty()) {
        const auto [src, dst, high] = q.front();
        q.pop();

        auto &mod = g.at(dst);
        if (ModuleType::FlipFlop == mod.type) {
            if (high) {
                continue;
            }
            auto &state = mod.state;
            state = !state;

            for (const auto &next: mod.outputs) {
                q.emplace(dst, next, state);
            }

            count[state ? 1 : 0] += mod.outputs.size();
        } else if (ModuleType::Conjunction == mod.type) {
            mod.inputs.at(src) = high;
            const auto state = !all_of(mod.inputs.begin(), mod.inputs.end(), [](const auto &it) { return it.second; });
            for (const auto &next: mod.outputs) {
                q.emplace(dst, next, state);
            }
            count[state ? 1 : 0] += mod.outputs.size();

            if (high) {
                mod.cycles.at(src) = step;
                if ((0 == mod.cycle) &&
                    all_of(mod.cycles.begin(), mod.cycles.end(), [](const auto &it) { return 0; })) {
                    mod.cycle = 1;
                }
                for (const auto &[_, cycle]: mod.cycles) {
                    mod.cycle = lcm(mod.cycle, cycle);
                }
            }
        }
    }
    return count;
}

static Graph toGraph(const vector<string> &lines) {
    Graph g;
    for (const auto &line: lines) {
        string name, out;
        vector<string> outputs;

        istringstream iss(line);
        iss >> name;
        ModuleType type{ModuleType::Undefined};
        if (name == "broadcaster") {
            type = ModuleType::Broadcast;
        } else if ('%' == name[0]) {
            type = ModuleType::FlipFlop;
            name = name.substr(1, name.size() - 1);
        } else if ('&' == name[0]) {
            type = ModuleType::Conjunction;
            name = name.substr(1, name.size() - 1);
        }
        iss.ignore(4);
        while (iss >> out) {
            if (',' == out[out.size() - 1]) {
                out = out.substr(0, out.size() - 1);
            }
            if (auto it = g.find(out); it != g.end()) {
                it->second.inputs[name] = false;
                it->second.cycles[name] = 0;
            } else {
                auto mod = Module{};
                mod.inputs[name] = false;
                mod.cycles[name] = 0;
                g[out] = mod;
            }
            outputs.emplace_back(out);
        }

        if (auto it = g.find(name); it != g.end()) {
            it->second.type = type;
            it->second.outputs = std::move(outputs);
        } else {
            auto mod = Module{};
            mod.type = type;
            mod.outputs = std::move(outputs);
            g[name] = mod;
        }
    }
    return g;
}

template<typename T, size_t N>
array<T, N> &operator+=(array<T, N> &lhs, const array<T, N> rhs) {
    for (size_t i = 0; i < N; ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

int main() {
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    auto g = toGraph(lines);

    {   // Part 1
        Count count{0, 0};
        for (uint64_t step = 1; step <= 1000; ++step) {
            count += simulate(g, step);
        }
        cout << "  Part 1" << endl;
        cout << "     Low * High : " << count[0] * count[1] << endl;
    }
    {  // Part 2
        cout << "  Part 2" << endl;
        cout << "     Use LCM : " << endl;
    }

    return EXIT_SUCCESS;
}