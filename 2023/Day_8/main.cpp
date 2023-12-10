// Advent of Code 2023
// Day 8:
// https://adventofcode.com/2023/day/7

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

using Map = map<string, pair<string, string>>;

string file1 = "input2.txt";

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

    Map map;

    // read the lookup into map
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    cout << "Day 8" << endl;

    // read the map from the lines
    for (uint32_t i = 2; i < lines.size(); ++i) {
        auto nodeName = lines[i].substr(0, 3);
        auto left = lines[i].substr(7, 3);
        auto right = lines[i].substr(12, 3);

        map[nodeName] = pair(left, right);
    }

    {
        if (lines[2].substr(0, 3) == "AAA") {
            // PART 1
            uint64_t count{};

            string nodeName = "AAA";
            do {
                const auto direction = lines[0][count % lines[0].size()];
                if ('L' == direction) {
                    nodeName = map.at(nodeName).first;
                } else {
                    nodeName = map.at(nodeName).second;
                }
                count++;
            } while (nodeName != "ZZZ");

            cout << "  Part 1" << endl;
            cout << "    Steps required to reach ZZZ : " << count << endl;
        }
    }

    {
        if (lines[2].substr(0, 3) != "AAA") {
            // PART 2
            vector<string> nodes;
            vector<string> endNodes;

            // store starting nodes and ending nodes
            for (const auto &[node, _]: map) {
                if (node[2] == 'A') {
                    nodes.push_back(node);
                }
                if (node[2] == 'Z') {
                    endNodes.push_back(node);
                }
            }

            auto cycles = vector<uint64_t>(nodes.size(), 0);
            uint64_t count{};

            do {
                const auto direction = lines[0][count % lines[0].size()];
                for (uint64_t i = 0; i < nodes.size(); ++i) {
                    if (cycles[i] > 0) {
                        continue;
                    }

                    auto &nodeName = nodes[i];
                    if ('L' == direction) {
                        nodeName = map.at(nodeName).first;
                    } else {
                        nodeName = map.at(nodeName).second;
                    }

                    if (find(endNodes.begin(), endNodes.end(), nodeName) != endNodes.end()) {
                        cycles[i] = count + 1;
                    }
                }
                count++;
            } while (!all_of(cycles.begin(), cycles.end(), [](auto c) { return c > 0; }));

            count = 1;
            for (unsigned long long cycle : cycles) {
                count =   lcm(count, cycle);
            }

            cout << "  Part 2" << endl;
            cout << "    Steps required to reach nodes ending in Z : " << count << endl;
        }
    }

    return EXIT_SUCCESS;
}