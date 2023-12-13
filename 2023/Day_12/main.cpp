#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstdint>

using namespace std;
constexpr int none = -1;

uint64_t collect(
        const string &grid,
        int i_grid,
        const vector<int> &sizes,
        int i_size,
        int remain,
        map<tuple<int, int, int>, uint64_t> &table) {
    const auto key = make_tuple(i_grid, i_size, remain);
    const auto found = table.find(key);
    if (found != table.cend()) { return found->second; }
    char ch;
    while (i_size != sizes.size() && i_grid != grid.size() && '?' != (ch = grid[i_grid])) {
        ++i_grid;
        if (ch == '.') {
            if (remain == none) {
                // do nothing
            } else if (remain == 0) {  // just finished a batch of #
                ++i_size;
                remain = none;
            } else {  // remain > 0
                table[key] = 0;
                return 0;  // impossible, so terminate
            }
        } else {  // ch == '#'
            if (remain == none) {
                remain = sizes[i_size] - 1;
            } else if (remain == 0) {
                table[key] = 0;
                return 0;  // impossible
            } else {  // remain > 0
                --remain;
            }
        }
    }
    if (i_size == sizes.size()) {
        for (int i = i_grid; i < grid.size(); ++i) {
            if (grid[i] == '#') {
                table[key] = 0;
                return 0;
            }
        }
        table[key] = 1;
        return 1;
    } else if (i_grid == grid.size()) {
        if (remain == 0 && i_size + 1 == sizes.size()) {
            table[key] = 1;
            return 1;
        }
    } else {  // ch == '?' and haven't finished yet
        // try ? = #
        uint64_t total = 0;
        if (remain == none) {
            const auto new_remain = sizes[i_size] - 1;
            total += collect(grid, 1 + i_grid, sizes, i_size, new_remain, table);
        } else if (remain > 0) {
            total += collect(grid, 1 + i_grid, sizes, i_size, remain - 1, table);
        }
        // try ? = .
        if (remain == 0) {
            total += collect(grid, 1 + i_grid, sizes, 1 + i_size, none, table);
        } else if (remain == none) {
            total += collect(grid, 1 + i_grid, sizes, i_size, none, table);
        }
        table[key] = total;
        return total;
    }
    table[key] = 0;
    return 0;
}

uint64_t solve(const string &grid, const vector<int> &sizes) {
    map<tuple<int, int, int>, uint64_t> table;
    return collect(grid, 0, sizes, 0, none, table);
}

struct Problem {
    string grid;
    vector<int> nums;
};

auto split_nums(const string &list) {
    size_t i = 0;
    vector<int> nums;
    while (true) {
        const auto k = list.find(',', i);
        if (k == string::npos) {
            nums.push_back(stoi(list.substr(i)));
            break;
        } else {
            nums.push_back(stoi(list.substr(i, k - i)));
            i = k + 1;
        }
    }
    return nums;
}

auto times_five_with_questions(const string &text) {
    string out = text;
    for (int i = 0; i < 4; ++i) {
        out.push_back('?');
        copy(text.cbegin(), text.cend(), back_inserter(out));
    }
    return out;
}

auto times_five(const vector<int> &sizes) {
    vector<int> out = sizes;
    for (int i = 0; i < 4; ++i) {
        copy(sizes.cbegin(), sizes.cend(), back_inserter(out));
    }
    return out;
}

int main() {
    vector<Problem> problems;
    if (auto fs = ifstream("input.txt")) {
        cout << "Day 9" << endl;

        uint64_t part1 = 0, part2 = 0;
        string grid, nums;
        while (fs >> grid) {
            fs >> nums;
            const auto sizes = split_nums(nums);
            part1 += solve(grid, sizes);
            const auto five_grid = times_five_with_questions(grid);
            const auto five_sizes = times_five(sizes);
            part2 += solve(five_grid, five_sizes);
        }
        cout << "  Part 1" << endl;
        cout << "     Sum of counts : " << part1 << endl;

        cout << "  Part 2" << endl;
        cout << "     New sum of counts : " << part2 << endl;
    }
}