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

// Advent of Code Day 23
// https://adventofcode.com/2023/day/23

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <format>
#include <algorithm>
#include <chrono>

using namespace std;

const string file1 = "input.txt";

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord {
    constexpr explicit Coord(T X = 0, T Y = 0) : x(X), y(Y) {}

    constexpr bool operator<(const Coord &p) const {
        if (x < p.x) return true;
        else if (p.x < x)
            return false;
        else return y < p.y;
    }

    constexpr Coord operator+(const Coord &p) const { return Coord(x + p.x, y + p.y); }

    constexpr bool operator==(const Coord &p) const { return x == p.x && y == p.y; }

    T x, y;
};

using Point = Coord<int>;
constexpr array<Point, 4> directions = {Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)};
constexpr array<char, 4> validSlopes = {'v', '>', '^', '<'};

template<typename T>
class y_combinator {
    T lambda;
public:
    constexpr explicit y_combinator(T &&t) : lambda(std::forward<T>(t)) {}

    template<typename...Args>
    constexpr decltype(auto) operator()(Args &&...args) const {
        return lambda(std::move(*this), std::forward<Args>(args)...);
    }
};

struct PathLength {
    Point start, end;
    int cost{};

    bool operator<(const PathLength &pl) const {
        if (start < pl.start) return true;
        if (pl.start < start) return false;
        return end < pl.end;
    }
};

int main() {

    ifstream in(file1);
    if (!in) {
        cout << format("Could not open inputFilename {}\n", file1);
        return EXIT_FAILURE;
    }

    chrono::time_point<std::chrono::system_clock> startTime, endTime;

    string line;
    vector<string> map;
    while (in >> line)
        map.emplace_back(std::move(line));

    vector<PathLength> pathSegments, bothDir;
    Point startPoint(0, 1), targetPoint(static_cast<int>(map.size() - 1), static_cast<int>(map.front().size() - 2));
    set<Point> toEval = {startPoint};

    while (!toEval.empty()) {
        Point pathStart = *toEval.cbegin();
        set<Point> cur = {pathStart}, next, repeatGuard = cur;
        toEval.erase(pathStart);
        int pathCost = 1;

        while (!cur.empty()) {
            for (Point p: cur) {
                char slope = map[p.x][p.y];
                for (int i = 0; i < 4; ++i) {
                    if (slope != '.' && slope != validSlopes[i])
                        continue;
                    if (Point np = p + directions[i];
                            np.x >= 0 && np.y >= 0 && np.x < map.size() && np.y < map.front().size())
                        if (char c = map[np.x][np.y];
                                c != '#') {
                            if (c == '.') {
                                if (repeatGuard.insert(np).second)
                                    next.insert(np);
                                if (np == targetPoint) {
                                    PathLength pl;
                                    pl.start = pathStart;
                                    pl.end = np;
                                    pl.cost = pathCost;
                                    pathSegments.push_back(pl);
                                }
                            } else if (c == validSlopes[i]) {
                                PathLength pl;
                                pl.start = pathStart;
                                pl.end = np;
                                pl.cost = pathCost;
                                pathSegments.push_back(pl);
                                toEval.insert(np);
                            }
                        }
                }
            }
            swap(next, cur);
            next.clear();
            ++pathCost;
        }
    }

    auto FindLongest = y_combinator([&](auto FindLongest, const Point &p) -> int {
        int maxSubCost = 0;
        for (const PathLength &pl: pathSegments)
            if (pl.start == p) {
                if (pl.end != targetPoint) {
                    int cost = FindLongest(pl.end);
                    maxSubCost = max(maxSubCost, cost + pl.cost);
                } else {
                    maxSubCost = max(maxSubCost, pl.cost);
                }
            }

        return maxSubCost;
    });

    startTime = chrono::system_clock::now();
    int part1 = FindLongest(startPoint), part2 = 0;

    auto IsSlope = [&](const Point &p) {
        char c = map[p.x][p.y];
        return find(ALLc(validSlopes), c) != validSlopes.cend();
    };

    vector<Point> crossings = {startPoint, targetPoint}, path = {startPoint};
    for (int i = 1; i < map.size() - 1; ++i)
        for (int j = 1; j < map.front().size() - 1; ++j)
            if (map[i][j] == '.') {
                array<Point, 4> dir = {Point(i - 1, j), Point(i + 1, j), Point(i, j - 1), Point(i, j + 1)};
                if (int count = IsSlope(dir[0]) + IsSlope(dir[1]) + IsSlope(dir[2]) + IsSlope(dir[3]);
                        count > 2) {
                    crossings.emplace_back(i, j);
                }
            }

    for (const Point &toEval: crossings) {
        if (toEval == targetPoint) continue;
        set<Point> cur = {toEval}, next, guard = cur;
        int cost = 1;
        while (!cur.empty()) {
            for (const Point &p: cur)
                for (const Point &d: directions)
                    if (Point np = p + d;
                            np.x >= 0 && np.y >= 0 && np.x < map.size() && np.y < map.front().size()) {
                        if (char c = map[np.x][np.y];
                                c != '#' && guard.insert(np).second) {
                            if (find(ALLc(crossings), np) != crossings.cend()) {
                                if (np == startPoint) continue;
                                PathLength pl;
                                pl.start = toEval;
                                pl.end = np;
                                pl.cost = cost;
                                bothDir.push_back(pl);
                            } else
                                next.insert(np);
                        }

                    }
            swap(cur, next);
            next.clear();
            ++cost;
        }
    }

    sort(ALL(bothDir));
    y_combinator([&](auto FindLongest2, vector<Point> &path, int runningCost) -> void {
        int i = 0;
        const Point &p = path.back();
        while (bothDir[i].start != p)
            ++i;

        for (; i < bothDir.size() && bothDir[i].start == p; ++i) {
            const PathLength &pl = bothDir[i];
            if (pl.end != targetPoint) {
                if (find(ALLc(path), pl.end) == path.cend()) {
                    path.push_back(pl.end);
                    FindLongest2(path, runningCost + pl.cost);
                    path.pop_back();
                }
            } else
                part2 = max(runningCost + pl.cost, part2);
        }
    })(path, 0);

    endTime = std::chrono::system_clock::now();
    chrono::duration<double>elapsed_seconds = endTime - startTime;

    cout << "  Part 1" << endl;
    cout << "     Steps in Longest Hike : " << part1 << endl;

    cout << "  Part 2" << endl;
    cout << "     Steps in Longest Hike with no slips: " << part2 << endl;

    cout << "Elapsed time : " << elapsed_seconds.count() << "s" << endl;

    return EXIT_SUCCESS;
}