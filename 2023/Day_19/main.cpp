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

// Advent of Code Day 19
// https://adventofcode.com/2023/day/19

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <array>
#include <memory>

using namespace std;

using RuleName = string;
using Index = uint8_t;
using Target = variant<monostate, RuleName, bool>;

const string file1 = "input.txt";

struct Part {
    Part(uint16_t x, uint16_t m, uint16_t a, uint16_t s) : items{array<uint16_t, 4>{x, m, a, s}} {
    }

    [[nodiscard]] uint64_t sum() const {
        return items[0] + items[1] + items[2] + items[3];
    }

    array<uint16_t, 4> items;
};

enum class RuleType {
    Undefined,
    Accept,
    Reject,
    Next,
    Less,
    Greater
};

struct Rule {
    [[nodiscard]] virtual Target eval(const Part &part) const {
        return {};
    }

    RuleType type{RuleType::Undefined};
};

struct Accept : Rule {
    Accept() {
        type = RuleType::Accept;
    }

    [[nodiscard]] Target eval(const Part &part) const override {
        return true;
    }
};

struct Reject : Rule {
    Reject() {
        type = RuleType::Reject;
    }

    [[nodiscard]] Target eval(const Part &part) const override {
        return false;
    }
};

struct Next : Rule {
    explicit Next(RuleName target) : target{std::move(target)} {
        type = RuleType::Next;
    }

    [[nodiscard]] Target eval(const Part &part) const override {
        return target;
    }

    RuleName target;
};

struct Less : Rule {
    Less(Index index, uint16_t amt, Target target) : index{index}, amt{amt}, target{std::move(target)} {
        type = RuleType::Less;
    }

    [[nodiscard]] Target eval(const Part &part) const override {
        if (part.items[index] < amt) {
            return target;
        }
        return {};
    }

    Index index;
    uint16_t amt;
    Target target;
};

struct Greater : Rule {
    Greater(Index index, uint16_t amt, Target target) : index{index}, amt{amt}, target{std::move(target)} {
        type = RuleType::Greater;
    }

    [[nodiscard]] Target eval(const Part &part) const override {
        if (part.items[index] > amt) {
            return target;
        }
        return {};
    }

    Index index;
    uint16_t amt;
    Target target;
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


using Parts = vector<Part>;
using Rules = vector<shared_ptr<Rule> >;
using RuleMap = map<RuleName, Rules>;
using Range = pair<uint16_t, uint16_t>;

struct RangePart {
    RangePart(Range x, Range m, Range a, Range s) : items{array<Range, 4>{x, m, a, s}} {
    }

    RangePart(const RangePart &other, Range range, Index index) {
        items = other.items;
        items[index] = range;
    }

    [[nodiscard]] uint64_t count() const {
        uint64_t prod{1};
        for (const auto &[s, e]: items) {
            prod *= (e - s);
        }
        return prod;
    }

    [[nodiscard]] bool isValid() const {
        for (const auto &[s, e]: items) {
            if (s >= e) {
                return false;
            }
        }
        return true;
    }

    array<Range, 4> items;
};

static pair<Parts, RuleMap> toPartsRules(const vector<string> &lines) {
    vector<Part> parts;
    RuleMap ruleMap;
    bool isParts{false};
    for (const auto &line: lines) {
        if (line.empty()) {
            isParts = true;
            continue;
        }
        if (isParts) {
            uint16_t x, m, a, s;
            sscanf(line.c_str(), "{x=%hu,m=%hu,a=%hu,s=%hu}", &x, &m, &a, &s);
            parts.emplace_back(x, m, a, s);
        } else {
            RuleName name;
            vector<shared_ptr<Rule> > conds;
            bool isCond{false};
            Index index;
            for (size_t i = 0; i < line.size(); ++i) {
                if ('{' == line[i]) {
                    name = line.substr(0, i);
                    isCond = true;
                    continue;
                }
                if (!isCond) {
                    continue;
                }
                if (const auto isOp = i < line.size() - 1 && !isalpha(line[i + 1]); isOp) {
                    if ('x' == line[i]) {
                        index = 0;
                        i++;
                    } else if ('m' == line[i]) {
                        index = 1;
                        i++;
                    } else if ('a' == line[i]) {
                        index = 2;
                        i++;
                    } else if ('s' == line[i]) {
                        index = 3;
                        i++;
                    }
                }
                const auto isLess = '<' == line[i];
                const auto isGreater = '>' == line[i];
                uint16_t amt{};
                if (isLess || isGreater) {
                    while (line[++i] != ':') {
                        amt = 10 * amt + line[i] - '0';
                    }
                    i++;
                }
                RuleName target;
                size_t start = i;
                while (i < line.size() && isalpha(line[i])) {
                    i++;
                }
                target = line.substr(start, i - start);
                const auto isA = target == "A";
                const auto isR = target == "R";
                if (isLess) {
                    if (isA) {
                        conds.emplace_back(make_shared<Less>(index, amt, true));
                    } else if (isR) {
                        conds.emplace_back(make_shared<Less>(index, amt, false));
                    } else {
                        conds.emplace_back(make_shared<Less>(index, amt, target));
                    }
                } else if (isGreater) {
                    if (isA) {
                        conds.emplace_back(make_shared<Greater>(index, amt, true));
                    } else if (isR) {
                        conds.emplace_back(make_shared<Greater>(index, amt, false));
                    } else {
                        conds.emplace_back(make_shared<Greater>(index, amt, target));
                    }
                } else if (isA) {
                    conds.emplace_back(make_shared<Accept>());
                } else if (isR) {
                    conds.emplace_back(make_shared<Reject>());
                } else {
                    conds.emplace_back(make_shared<Next>(target));
                }
            }
            ruleMap[name] = std::move(conds);
        }
    }
    return {parts, ruleMap};
}

static bool validate(const RuleName &ruleName, const RuleMap &ruleMap, const Part &part, size_t ruleIndex = 0) {
    const auto &rules = ruleMap.at(ruleName);
    const auto result = rules[ruleIndex]->eval(part);
    if (holds_alternative<RuleName>(result)) {
        return validate(get<RuleName>(result), ruleMap, part);
    }
    if (holds_alternative<bool>(result)) {
        return get<bool>(result);
    }
    return validate(ruleName, ruleMap, part, ruleIndex + 1);
}

static uint64_t count(const RuleName &ruleName, const RuleMap &ruleMap, const RangePart &part, size_t ruleIndex = 0) {
    if (!part.isValid()) {
        return 0;
    }
    const auto &rules = ruleMap.at(ruleName);
    const auto &baseRule = rules[ruleIndex];
    if (RuleType::Accept == baseRule->type) {
        return part.count();
    }
    if (RuleType::Reject == baseRule->type) {
        return 0;
    }
    if (RuleType::Next == baseRule->type) {
        const auto rule = dynamic_pointer_cast<Next>(baseRule);
        return count(rule->target, ruleMap, part);
    }
    if (RuleType::Less == baseRule->type) {
        const auto rule = dynamic_pointer_cast<Less>(baseRule);
        const auto os = part.items[rule->index].first;
        const auto oe = min<uint16_t>(rule->amt, part.items[rule->index].second);
        const auto &result = rule->target;
        const auto &oPart = RangePart(part, {os, oe}, rule->index);                              // overlap range
        const auto &rPart = RangePart(part, {oe, part.items[rule->index].second}, rule->index);  // right range
        const auto rSum = count(ruleName, ruleMap, rPart, ruleIndex + 1);
        if (holds_alternative<RuleName>(result)) {
            return count(get<RuleName>(result), ruleMap, oPart) + rSum;
        } else if (holds_alternative<bool>(result)) {
            return (get<bool>(result) ? oPart.count() : 0) + rSum;
        }
        return 0;
    }
    if (RuleType::Greater == baseRule->type) {
        const auto rule = dynamic_pointer_cast<Greater>(baseRule);
        const auto os = max<uint16_t>(rule->amt + 1, part.items[rule->index].first);
        const auto oe = part.items[rule->index].second;
        const auto &result = rule->target;
        const auto &oPart = RangePart(part, {os, oe}, rule->index);                             // overlap range
        const auto &lPart = RangePart(part, {part.items[rule->index].first, os}, rule->index);  // left range
        const auto lSum = count(ruleName, ruleMap, lPart, ruleIndex + 1);
        if (holds_alternative<RuleName>(result)) {
            return count(get<RuleName>(result), ruleMap, oPart) + lSum;
        } else if (holds_alternative<bool>(result)) {
            return (get<bool>(result) ? oPart.count() : 0) + lSum;
        }
        return 0;
    }
    return 0;
}

int main() {
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    cout << "Day 19" << endl;
    const auto &[parts, ruleMap] = toPartsRules(lines);

    {  // Part 1
        uint64_t sum{};
        for (const auto &part: parts) {
            if (validate(RuleName{"in"}, ruleMap, part)) {
                sum += part.sum();
            }
        }
        cout << "  Part 1" << endl;
        cout << "     Total : " << sum << endl;
    }
    {  // Part 2
        constexpr auto range = Range{1, 4001};
        const auto sum = count(RuleName{"in"}, ruleMap, RangePart{range, range, range, range});
        cout << "  Part 2" << endl;
        cout << "     Total : " << sum << endl;
    }

    return EXIT_SUCCESS;
}