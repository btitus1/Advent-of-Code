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
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
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

using V = array<int64_t, 3>;
using Line = array<V, 2>;
using Lines = vector<Line>;

template<typename T, size_t N>
array<T, N> operator-(const array<T, N> &a, const array<T, N> &b) {
    array<T, N> result{};
    for (size_t i = 0; i < N; ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template<typename INTERSECT_TYPE>
static optional<array<INTERSECT_TYPE, 2> > intersect(const Line &a, const Line &b) {
    const auto &p1 = a[0];
    const auto &v1 = a[1];
    const auto &p2 = b[0];
    const auto &v2 = b[1];
    INTERSECT_TYPE s, t;
    const auto den1 = v2[0] * v1[1];
    const auto den2 = v2[1] * v1[0];
    if (den1 == den2) {
        // parallel or identical
        return nullopt;
    } else {
        const auto nom1 = ((p2[1] - p1[1]) * v1[0] + (p1[0] - p2[0]) * v1[1]);
        if constexpr (is_integral_v<INTERSECT_TYPE>) {
            if (0 != nom1 % (den1 - den2)) {
                // not an integer solution
                return nullopt;
            }
        }
        s = nom1 / static_cast<INTERSECT_TYPE>(den1 - den2);
        const auto nom2 = (p2[0] - p1[0]) + s * v2[0];
        t = nom2 / static_cast<INTERSECT_TYPE>(v1[0]);
    }
    return array<INTERSECT_TYPE, 2>{t, s};
}

template<typename INTERSECT_TYPE>
static bool checkIntersect(const Line &a, const Line &b, int64_t start, int64_t end, bool ignoreZ) {
    const auto ts = intersect<INTERSECT_TYPE>(a, b);
    const auto &p1 = a[0];
    const auto &v1 = a[1];
    const auto &p2 = b[0];
    const auto &v2 = b[1];
    if (!ts.has_value()) {
        const auto isParallel = (p2[0] - p1[0]) * v1[1] == (p2[1] - p1[1]) * v1[0];
        if (ignoreZ) {
            return isParallel;
        } else {
            return isParallel || (p2[0] - p1[0]) * v1[2] == (p2[2] - p1[2]) * v1[0];
        }
    }
    const auto [t, s] = ts.value();
    auto isInter = s >= 0 && t >= 0;
    if constexpr (is_integral_v<INTERSECT_TYPE>) {
        if (isInter && !ignoreZ) {
            const auto r1 = p1[2] + t * v1[2];
            const auto r2 = p2[2] + s * v2[2];
            isInter = isInter && r1 == r2;
        }
    } else {
        for (uint8_t i = 0; i < 3; ++i) {
            if (!isInter) {
                break;
            }
            if (ignoreZ && 2 == i) {
                break;
            }
            const auto r1 = p1[i] + t * v1[i];
            const auto r2 = p2[i] + s * v2[i];
            isInter = isInter && r1 >= start && r2 >= start;
            isInter = isInter && r1 <= end && r2 <= end;
        }
    }
    return isInter;
}

template<typename INTERSECT_TYPE>
static size_t countIntersect(const Lines &lines, int64_t start, int64_t end, bool ignoreZ = true) {
    size_t count{};
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = i + 1; j < lines.size(); ++j) {
            if (checkIntersect<INTERSECT_TYPE>(lines[i], lines[j], start, end, ignoreZ)) {
                count++;
            }
        }
    }
    return count;
}

template<typename INTERSECT_TYPE>
static bool checkIntersect(const Lines &lines, const V &vDiff, bool ignoreZ = true) {
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = i + 1; j < lines.size(); ++j) {
            const auto &pi = lines[i][0];
            const auto &vi = lines[i][1] - vDiff;
            const auto &pj = lines[j][0];
            const auto &vj = lines[j][1] - vDiff;
            if (!checkIntersect<INTERSECT_TYPE>({pi, vi}, {pj, vj}, INT64_MIN, INT64_MAX, ignoreZ)) {
                return false;
            }
        }
    }
    return true;
}

static V findRock(const Lines &lines) {
    constexpr int64_t vRange = 300;
    for (int64_t v1 = -vRange; v1 <= vRange; ++v1) {
        for (int64_t v2 = -vRange; v2 <= vRange; ++v2) {
            if (const auto v = V{v1, v2, 0}; !checkIntersect<int64_t>(lines, v, true)) {
                continue;
            }
            for (int64_t v3 = -vRange; v3 <= vRange; ++v3) {
                if (const auto v = V{v1, v2, v3}; checkIntersect<int64_t>(lines, v, false)) {
                    // get the actual intersection point (from any two lines)
                    const auto &pi = lines[0][0];
                    const auto &vi = lines[0][1] - v;
                    const auto &pj = lines[1][0];
                    const auto &vj = lines[1][1] - v;
                    const auto &[t, s] = intersect<int64_t>({pi, vi}, {pj, vj}).value();
                    return V{pi[0] + t * vi[0], pi[1] + t * vi[1], pi[2] + t * vi[2]};
                }
            }
        }
    }
    return V{0, 0, 0};
}

int main(int argc, char *argv[]) {
    vector<string> lines{};
    if (!readFile(file1, lines)) {
        return EXIT_FAILURE;
    }

    Lines ls;
    chrono::time_point<chrono::system_clock> startTime, endTime;

    {  // Part 1
        for (const auto &line: lines) {
            Line l;
            istringstream iss{line};
            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 3; ++j) {
                    int64_t x;
                    iss >> x;
                    l[i][j] = x;
                    iss.ignore(1);
                }
                iss.ignore(2);
            }
            ls.push_back(l);
        }
        cout << "  Part 1" << endl;
        startTime = chrono::system_clock::now();
        const auto count = countIntersect<double>(ls, 200000000000000, 400000000000000);
        endTime = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;
        cout << "     Intersections in test area : " << count << endl;
        cout << "     Elapsed time : " << elapsed_seconds.count() << "s" << endl;
    }
    {  // Part 2
        cout << "  Part 2" << endl;
        startTime = chrono::system_clock::now();
        const auto rock = findRock(ls);
        endTime = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = endTime - startTime;
        cout << "     Coordinates : [" << rock[0] << "," << rock[1] << "," << rock[2] << "]" << endl;
        cout << "             Sum :  " << rock[0] + rock[1] + rock[2] << endl;
        cout << "     Elapsed time : " << elapsed_seconds.count() << "s" << endl;
    }

    return EXIT_SUCCESS;
}