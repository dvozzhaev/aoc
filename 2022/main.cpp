// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <regex>
#include <map>
#include <deque>
#include <functional>
#include <optional>
#include <variant>
#include <list>

std::vector<std::string> read_input(std::string path) {
    std::vector<std::string> result;
    std::ifstream input("input/" + path);
    while (input.good()) {
        std::string line;
        std::getline(input, line);
        result.push_back(line);
    }
    return result;
}

template<typename T>
T sign(T a) {
    return a < 0 ? -1 : a > 0 ? 1 : 0;
}

struct pt {
    int x, y;
    pt() : pt(0, 0) {}
    pt(std::string x, std::string y) : pt(std::stoi(x), std::stoi(y)) {}
    pt(int x, int y) : x(x), y(y) {}
    
    bool operator == (const pt& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator < (const pt& other) const {
        return x != other.x ? x < other.x : y < other.y;
    }
    
    pt operator -(const pt& other) const {
        return pt(x - other.x, y - other.y);
    }
};

int day1_1(std::vector<std::string> lines) {
    int n = 0;
    int max = 0;
    for (auto& line : lines) {
        if (line.empty()) {
            max = std::max(n, max);
            n = 0;
            continue;
        }
        n += std::stoi(line);
    }
    return max;
}

int day1_2(std::vector<std::string> lines) {
    std::vector<int> elves;
    int n = 0;
    for (auto& line : lines) {
        if (line.empty()) {
            elves.push_back(n);
            n = 0;
            continue;
        }
        n += std::stoi(line);
    }
    std::sort(elves.rbegin(), elves.rend());
    return elves[0] + elves[1] + elves[2];
}

int day2_1(std::vector<std::string> lines) {
    int score = 0;
    for (auto& line : lines) {
        enum Move { ROCK = 0, PAPER = 1, SCISSOR = 2 };
        Move tho = static_cast<Move>(line[0] - 'A');
        Move you = static_cast<Move>(line[2] - 'X');
        bool draw = tho == you;
        bool win = draw ? false : (tho == ROCK && you == PAPER) || (tho == PAPER && you == SCISSOR) || (tho == SCISSOR && you == ROCK);
        int value = you == ROCK ? 1 : you == PAPER ? 2 : 3;
        int r = (win ? 6 : draw ? 3 : 0);
        score += r + value;
    }
    return score;
}

int day2_2(std::vector<std::string> lines) {
    int score = 0;
    for (auto& line : lines) {
        enum Move { ROCK = 0, PAPER = 1, SCISSOR = 2 };
        enum Result { LOSE = 0, DRAW = 1, WIN = 2 };
        Move tho = static_cast<Move>(line[0] - 'A');
        Result result = static_cast<Result>(line[2] - 'X');
        bool draw = result == DRAW;
        bool win = result == WIN;
        Move you = draw ? tho : win ? (tho == ROCK ? PAPER : tho == PAPER ? SCISSOR : ROCK) : (tho == ROCK ? SCISSOR : tho == PAPER ? ROCK : PAPER);
        int value = you == ROCK ? 1 : you == PAPER ? 2 : 3;
        int r = (win ? 6 : draw ? 3 : 0);
        score += r + value;
    }
    return score;
}

int day3_1(std::vector<std::string> lines) {
    int cost = 0;
    for (auto& line : lines) {
        std::vector<char> uniq;
        std::set<char> left(line.begin(), line.end() - line.size() / 2);
        std::set<char> right(line.begin() + line.size() / 2, line.end());
        std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(uniq));
        if (uniq.size() != 1) {
            return -1;
        }
        char u = uniq[0];
        if (u >= 'a' && u <= 'z') cost += u - 'a' + 1;
        else if (u >= 'A' && u <= 'Z') cost += u - 'A' + 27;
        else return -2;
    }
    return cost;
}

int day3_2(std::vector<std::string> lines) {
    int cost = 0;
    std::vector<std::set<char>> uniq;
    for (auto& line : lines) {
        uniq.emplace_back(line.begin(), line.end());
    }
    for (size_t i = 0; i < lines.size(); i += 3) {
        std::vector<char> u2;
        std::set_intersection(uniq[i].begin(), uniq[i].end(), uniq[i + 1].begin(), uniq[i + 1].end(), std::back_inserter(u2));
        std::vector<char> u3;
        std::set_intersection(uniq[i + 2].begin(), uniq[i + 2].end(), u2.begin(), u2.end(), std::back_inserter(u3));
        if (u3.size() != 1) {
            return -1;
        }
        char u = u3[0];
        if (u >= 'a' && u <= 'z') cost += u - 'a' + 1;
        else if (u >= 'A' && u <= 'Z') cost += u - 'A' + 27;
        else return -2;
    }
    return cost;
}

namespace day4 {
    struct range {
        int left, right;
    };
    std::pair<range, range> split(std::string& line) {
        std::regex r("(\\d*)-(\\d*),(\\d*)-(\\d*)");
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        int a1 = std::stoi(it[1]);
        int a2 = std::stoi(it[2]);
        int b1 = std::stoi(it[3]);
        int b2 = std::stoi(it[4]);
        return { {a1, a2}, {b1, b2} };
    }

    int first(std::vector<std::string> lines) {
        int c = 0;
        for (auto& line : lines) {
            auto [left, right] = split(line);
            if (left.left <= right.left && left.right >= right.right) c++;
            else if (left.left >= right.left && left.right <= right.right) c++;
        }
        return c;
    }

    int second(std::vector<std::string> lines) {
        int c = 0;
        for (auto& line : lines) {
            auto [left, right] = split(line);
            if (left.right - left.left > right.right - right.left) std::swap(left, right);
            if ((left.left >= right.left && left.left <= right.right)
                || (left.right >= right.left && left.right <= right.right)) c++;
        }
        return c;
    }
}


std::string day5_1(std::vector<std::string> lines) {
    std::vector<std::string> input = {
        "[V]         [T]         [J]        ",
        "[Q]         [M] [P]     [Q]     [J]",
        "[W] [B]     [N] [Q]     [C]     [T]",
        "[M] [C]     [F] [N]     [G] [W] [G]",
        "[B] [W] [J] [H] [L]     [R] [B] [C]",
        "[N] [R] [R] [W] [W] [W] [D] [N] [F]",
        "[Z] [Z] [Q] [S] [F] [P] [B] [Q] [L]",
        "[C] [H] [F] [Z] [G] [L] [V] [Z] [H]",
    };

    std::map<int, std::vector<char>> stacks;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 9; ++j) {
            char c = input[i][j * 4 + 1];
            if (c >= 'A' && c <= 'Z') {
                stacks[j + 1].insert(stacks[j + 1].begin(), c);
            }
        }
    }

    for (auto& line : lines) {
        std::regex r("move (\\d*) from (\\d*) to (\\d*)");
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        int n = std::stoi(it[1]);
        int s = std::stoi(it[2]);
        int d = std::stoi(it[3]);
        for (int i = 0; i < n; ++i) {
            char c = stacks[s].back();
            stacks[d].push_back(c);
            stacks[s].pop_back();
        }
    }

    std::string r;
    for (auto [k, v] : stacks) {
        r += v.back();
    }
    return r;
}

std::string day5_2(std::vector<std::string> lines) {
    std::vector<std::string> input = {
        "[V]         [T]         [J]        ",
        "[Q]         [M] [P]     [Q]     [J]",
        "[W] [B]     [N] [Q]     [C]     [T]",
        "[M] [C]     [F] [N]     [G] [W] [G]",
        "[B] [W] [J] [H] [L]     [R] [B] [C]",
        "[N] [R] [R] [W] [W] [W] [D] [N] [F]",
        "[Z] [Z] [Q] [S] [F] [P] [B] [Q] [L]",
        "[C] [H] [F] [Z] [G] [L] [V] [Z] [H]",
    };
    std::reverse(input.begin(), input.end());
    std::map<int, std::vector<char>> stacks;
    for (auto& line: input) {
        for (int j = 0; j <= line.size() / 4; ++j) {
            char c = line[j * 4 + 1];
            if (c >= 'A' && c <= 'Z') {
                stacks[j + 1].push_back(c);
            }
        }
    }

    for (auto& line : lines) {
        std::regex r("move (\\d*) from (\\d*) to (\\d*)");
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        int n = std::stoi(it[1]);
        int s = std::stoi(it[2]);
        int d = std::stoi(it[3]);
        stacks[d].insert(stacks[d].end(), stacks[s].end() - n, stacks[s].end());
        stacks[s].erase(stacks[s].end() - n, stacks[s].end());
    }

    std::string r;
    for (auto [k, v] : stacks) {
        r += v.back();
    }
    return r;
}

int day6(std::vector<std::string> input, int N) {
    for (const auto& line : input) {
        for (int k = 0; k < line.size() - N; ++k) {
            int n = 0;
            for (int i = 0; i < N - 1; ++i) {
                for (int j = i + 1; j < N; ++j) {
                    if (line[k + i] == line[k + j])
                        ++n;
                }
            }
            if (n == 0) {
                return k + N;
            }
        }
    }
    return 0;
}

struct day7 {
    struct dir {
        int size;
        std::map<std::string, int> files;
        std::map<std::string, std::unique_ptr<dir>> dirs;
        dir* parent;

        dir(dir* parent) : parent(parent) {}
    };

    std::unique_ptr<dir> parse_input(std::vector<std::string> lines) {
        std::unique_ptr<dir> root = std::make_unique<dir>(nullptr);
        dir* cur = root.get();
        for (const auto& line : lines) {
            if (line.starts_with("$ cd ")) {
                if (line == "$ cd ..") {
                    cur = cur->parent;
                }
                else {
                    std::string n(line.begin() + 5, line.end());
                    if (cur->dirs.find(n) == cur->dirs.end()) {
                        cur->dirs[n] = std::make_unique<dir>(cur);
                    }
                    cur = cur->dirs[n].get();
                }
            }
            else if (line == "$ ls") {

            }
            else {
                if (line.starts_with("dir")) {
                    std::string n(line.begin() + 4, line.end());
                    if (cur->dirs.find(n) == cur->dirs.end()) {
                        cur->dirs[n] = std::make_unique<dir>(cur);
                    }
                }
                else {
                    size_t p = line.find(' ');
                    int s = std::stoi(std::string(line.begin(), line.begin() + p));
                    std::string n(line.begin() + p + 1, line.end());
                    cur->files[n] = s;
                }
            }
        }
        return root;
    }

    int update_size(dir* d) {
        int size = 0;
        for (auto& [k, v] : d->files) {
            size += v;
        }
        for (auto& [k, v] : d->dirs) {
            size += update_size(v.get());
        }
        d->size = size;
        return size;
    }

    void collect_small(dir* d, std::vector<dir*>& r) {
        if (d->size < 100000) {
            r.push_back(d);
        }
        for (auto& [k, v] : d->dirs) {
            collect_small(v.get(), r);
        }
    }

    void flatten(dir* d, std::vector<std::pair<int, dir*>>& r) {
        r.push_back(std::make_pair(d->size, d));
        for (auto& [k, v] : d->dirs) {
            flatten(v.get(), r);
        }
    }

    int first(std::vector<std::string> lines) {
        std::unique_ptr<dir> root = parse_input(lines);
        update_size(root.get());
        std::vector<dir*> small;
        collect_small(root.get(), small);
        int size = 0;
        for (dir* d : small) {
            size += d->size;
        }
        return size;
    }

    int second(std::vector<std::string> lines) {
        std::unique_ptr<dir> root = parse_input(lines);
        update_size(root.get());
        std::vector<std::pair<int, dir*>> dirs;
        flatten(root.get(), dirs);
        int total = 70000000;
        int need = 30000000;
        int have = total - root->size;
        std::sort(dirs.begin(), dirs.end());
        for (auto& [k, v] : dirs) {
            if (have + k >= need) return k;
        }
        return -1;
    }
};

int day8_1(std::vector<std::string> lines) {
    int r = 0;
    std::vector<std::vector<int>> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    int w = static_cast<int>(lines.size());
    int h = static_cast<int>(lines[0].size());
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            for (int i = 0; i < d.size(); ++i) {
                bool visible = true;
                int x1 = x, y1 = y;
                int t = lines[x][y];
                while (true) {
                    x1 += d[i][0];
                    y1 += d[i][1];
                    if (x1 < 0 || x1 >= w || y1 < 0 || y1 >= h) break;
                    if (lines[x1][y1] >= t) {
                        visible = false;
                        break;
                    }
                }
                if (visible) {
                    r += 1;
                    break;
                }
            }
        }
    }
    return r;
}

int day8_2(std::vector<std::string> lines) {
    int r = 0;
    std::vector<std::vector<int>> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    int w = static_cast<int>(lines.size());
    int h = static_cast<int>(lines[0].size());
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            int score = 1;
            for (int i = 0; i < d.size(); ++i) {
                int x1 = x, y1 = y;
                int t = lines[x][y];
                int view = 0;
                while (true) {
                    x1 += d[i][0];
                    y1 += d[i][1];
                    if (x1 < 0 || x1 >= w || y1 < 0 || y1 >= h) break;
                    view += 1;
                    if (lines[x1][y1] >= t) break;
                }
                score *= view;
            }
            r = std::max(r, score);
        }
    }
    return r;
}

namespace day9 {
    int day9_1(std::vector<std::string> lines) {
        std::set<std::pair<int, int>> t;
        int xh = 0, xt = 0, yh = 0, yt = 0;
        std::vector<std::vector<int>> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

        for (const auto& line : lines) {
            int di;
            switch (line[0]) {
            case 'U': di = 2; break;
            case 'D': di = 3; break;
            case 'L': di = 0; break;
            case 'R': di = 1; break;
            default: return -1;
            }
            int n = std::stoi(std::string(line.begin() + 2, line.end()));
            for (int i = 0; i < n; ++i) {
                xh += d[di][0];
                yh += d[di][1];
                /*
                012
                123
                23.
                */
                switch (abs(xt - xh) + abs(yt - yh)) {
                case 0: break;
                case 1: break;
                case 2:
                    if (xh == xt) {
                        yt += sign(yh - yt);
                    }
                    else if (yh == yt) {
                        xt += sign(xh - xt);
                    }
                    break;
                case 3:
                    xt += sign(xh - xt);
                    yt += sign(yh - yt);
                    break;
                default:
                    return -2;
                }

                t.insert(std::make_pair(xt, yt));
            }
        }
        return static_cast<int>(t.size());
    }

    std::pair<int, int> move(int di, std::pair<int, int> p) {
        static std::vector<std::vector<int>> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        return std::make_pair(p.first + d[di][0], p.second + d[di][1]);
    }

    std::pair<int, int> follow(std::pair<int, int> h, std::pair<int, int> t) {

        switch (abs(h.first - t.first) + abs(h.second - t.second)) {
        case 0: return t;
        case 1: return t;
        case 2:
            if (h.first == t.first) {
                t.second += sign(h.second - t.second);
            }
            else if (h.second == t.second) {
                t.first += sign(h.first - t.first);
            }
            break;
        case 3:
        case 4:
            t.first += sign(h.first - t.first);
            t.second += sign(h.second - t.second);
            break;
        default:
            throw "wtf";
        }
        return t;
    }

    int day9_2(std::vector<std::string> lines) {
        std::set<std::pair<int, int>> t;
        std::vector<std::pair<int, int>> rope(10, { 0 , 0 });
        for (const auto& line : lines) {
            int di;
            switch (line[0]) {
            case 'U': di = 2; break;
            case 'D': di = 3; break;
            case 'L': di = 0; break;
            case 'R': di = 1; break;
            default: return -1;
            }
            int n = std::stoi(std::string(line.begin() + 2, line.end()));
            for (int i = 0; i < n; ++i) {
                rope[0] = move(di, rope[0]);
                for (int j = 0; j < rope.size() - 1; ++j) {
                    rope[j + 1] = follow(rope[j], rope[j + 1]);
                }
                t.insert(rope.back());
            }
        }
        return static_cast<int>(t.size());
    }
}

int day10_1(std::vector<std::string> lines) {
    int i = 0;
    int X = 1, C = 1;
    bool half_cycle = false;
    int signal = 0;
    while (i < lines.size()) {
        if ((C - 20) % 40 == 0) {
            int S = C * X;
            signal += S;
        }
        if (lines[i] == "noop") { ++i; }
        else if (lines[i].starts_with("addx")) {
            if (half_cycle) {
                half_cycle = false;
                X += std::stoi(std::string(lines[i].begin() + 5, lines[i].end()));
                ++i;
            }
            else {
                half_cycle = true;
            }
        }
        else {
            return -1;
        }

        C++;
    }

    return signal;
}

std::string day10_2(std::vector<std::string> lines) {
    int i = 0;
    int X = 1, C = 0;
    bool half_cycle = false;
    std::string r;

    while (i < lines.size()) {
        if (C % 40 == 0) {
            r += '\n';
        }
        r += ((abs(X - (C % 40)) < 2) ? '#' : '.');
        if (lines[i] == "noop") { ++i; }
        else if (lines[i].starts_with("addx")) {
            if (half_cycle) {
                half_cycle = false;
                X += std::stoi(std::string(lines[i].begin() + 5, lines[i].end()));
                ++i;
            }
            else {
                half_cycle = true;
            }
        }
        else {
            throw "invalid op";
        }
        C++;
    }
    return r;
}

int day11_1() {
    struct Monkey {
        int test, left, right;
        std::function<int(int)> op;
        std::vector<int> items;
        int score;
    };
    /*
    std::vector<Monkey> monkeys = {
        {23, 2, 3, [](int n) { return n * 19; }, { 79, 98 }, },
        {19, 2, 0, [](int n) { return n + 6;  }, { 54, 65, 75, 74 }, },
        {13, 1, 3, [](int n) { return n * n;  }, { 79, 60, 97 }, },
        {17, 0, 1, [](int n) { return n + 3;  }, { 74 }, },
    };
    */
    std::vector<Monkey> monkeys = {
        { 2, 1, 6,  [](int n) { return n * 17; }, { 83, 62, 93                     } },
        { 17, 6, 3, [](int n) { return n + 1;  }, { 90, 55                         } },
        { 19, 7, 5, [](int n) { return n + 3;  }, { 91, 78, 80, 97, 79, 88         } },
        { 3, 7, 2,  [](int n) { return n + 5;  }, { 64, 80, 83, 89, 59             } },
        { 5, 0, 1,  [](int n) { return n * n;  }, { 98, 92, 99, 51                 } },
        { 13, 4, 0, [](int n) { return n + 2;  }, { 68, 57, 95, 85, 98, 75, 98, 75 } },
        { 7, 3, 2,  [](int n) { return n + 4;  }, { 74                             } },
        { 11, 4, 5, [](int n) { return n * 19; }, { 68, 64, 60, 68, 87, 80, 82     } },
    };

    for (int i = 0; i < 20; ++i) {
        for (auto& monkey : monkeys) {
            for (int item : monkey.items) {
                item = monkey.op(item) / 3;
                int next = item % monkey.test == 0 ? monkey.left : monkey.right;
                monkeys[next].items.push_back(item);
                ++monkey.score;
            }
            monkey.items.clear();
        }
    }

    std::vector<int> scores;
    for (auto& monkey : monkeys) {
        scores.push_back(monkey.score);
    }
    std::sort(scores.rbegin(), scores.rend());
    return scores[0] * scores[1];
}

int64_t day11_2() {
    struct Monkey {
        int test, left, right;
        std::function<int64_t(int64_t)> op;
        std::vector<int64_t> items;
        int score;
    };
    
    /*
    std::vector<Monkey> monkeys = {
        {23, 2, 3, [](int64_t n) { return n * 19; }, { 79, 98 }, },
        {19, 2, 0, [](int64_t n) { return n + 6;  }, { 54, 65, 75, 74 }, },
        {13, 1, 3, [](int64_t n) { return n * n;  }, { 79, 60, 97 }, },
        {17, 0, 1, [](int64_t n) { return n + 3;  }, { 74 }, },
    };
    */

    std::vector<Monkey> monkeys = {
        { 2, 1, 6,  [](int64_t n) { return n * 17; }, { 83, 62, 93                     } },
        { 17, 6, 3, [](int64_t n) { return n + 1;  }, { 90, 55                         } },
        { 19, 7, 5, [](int64_t n) { return n + 3;  }, { 91, 78, 80, 97, 79, 88         } },
        { 3, 7, 2,  [](int64_t n) { return n + 5;  }, { 64, 80, 83, 89, 59             } },
        { 5, 0, 1,  [](int64_t n) { return n * n;  }, { 98, 92, 99, 51                 } },
        { 13, 4, 0, [](int64_t n) { return n + 2;  }, { 68, 57, 95, 85, 98, 75, 98, 75 } },
        { 7, 3, 2,  [](int64_t n) { return n + 4;  }, { 74                             } },
        { 11, 4, 5, [](int64_t n) { return n * 19; }, { 68, 64, 60, 68, 87, 80, 82     } },
    };
    int n = 1;
    for (auto& monkey : monkeys) {
        n *= monkey.test;
    }

    for (int i = 0; i < 10000; ++i) {
        for (auto& monkey : monkeys) {
            for (int64_t item : monkey.items) {
                item = monkey.op(item) % (n);
                int next = item % monkey.test == 0 ? monkey.left : monkey.right;
                monkeys[next].items.push_back(item);
                ++monkey.score;
            }
            monkey.items.clear();
        }
    }

    std::vector<int64_t> scores;
    for (auto& monkey : monkeys) {
        scores.push_back(monkey.score);
    }
    std::sort(scores.rbegin(), scores.rend());
    return scores[0] * scores[1];
}

int day12_1(std::vector<std::string> lines) {
    using pt = std::pair<int, int>;
    std::vector<pt> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    std::map<pt, int> path;
    std::deque<pt> q;
    int w = static_cast<int>(lines.size());
    int h = static_cast<int>(lines[0].size());
    pt end;
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (lines[x][y] == 'S') {
                lines[x][y] = 'a';
                q.push_back({ x, y });
                path[{ x, y }] = 0;
            }
            if (lines[x][y] == 'E') {
                lines[x][y] = 'z';
                end = { x, y };
            }
        }
    }
    while (!q.empty()) {
        pt p = q.front();
        if (p == end) {
            break;
        }
        q.pop_front();
        int v = path[p];
        for (pt i : d) {
            pt n = { p.first + i.first, p.second + i.second };
            if (n.first < 0 || n.first >= w || n.second < 0 || n.second >= h) continue;
            if (lines[n.first][n.second] - 1 > lines[p.first][p.second]) continue;
            if (path.find(n) == path.end()) {
                path[n] = v + 1;
                q.push_back(n);
            }
        }
    }
    return path[end];
}

int day12_2(std::vector<std::string> lines) {
    using pt = std::pair<int, int>;
    std::vector<pt> d = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    std::map<pt, int> path;
    std::deque<pt> q;
    int w = static_cast<int>(lines.size());
    int h = static_cast<int>(lines[0].size());
    pt end;
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (lines[x][y] == 'S' || lines[x][y] == 'a') {
                lines[x][y] = 'a';
                q.push_back({ x, y });
                path[{ x, y }] = 0;
            }
            if (lines[x][y] == 'E') {
                lines[x][y] = 'z';
                end = { x, y };
            }
        }
    }
    while (!q.empty()) {
        pt p = q.front();
        if (p == end) {
            break;
        }
        q.pop_front();
        for (pt i : d) {
            pt n = { p.first + i.first, p.second + i.second };
            if (n.first < 0 || n.first >= w || n.second < 0 || n.second >= h) continue;
            if (lines[n.first][n.second] - 1 > lines[p.first][p.second]) continue;
            if (path.find(n) == path.end()) {
                path[n] = path[p] + 1;
                q.push_back(n);
            }
        }
    }
    return path[end];
}

struct Packet {
    std::variant<int, std::vector<Packet>> storage;

    Packet(int v) : storage(v) {}
    Packet(std::vector<Packet> v) : storage(v) {}

    bool is_value() const {
        return storage.index() == 0;
    }
    
    int as_value() const {
        return std::get<0>(storage);
    }

    std::vector<Packet> as_list() const {
        return is_value() ? std::vector<Packet>(1, as_value()) : std::get<1>(storage);
    }

    std::string to_string() const {
        if (is_value()) return std::to_string(as_value());
        std::string r = "[";
        for (auto& value : as_list()) {
            if (r.size() > 1) r += ",";
            r += value.to_string();
        }
        r += "]";
        return r;
    }
};

enum class Order {
    Correct, Wrong, Unknown
};

Order is_ordered(const Packet& a, const Packet& b) {
    if (a.is_value() && b.is_value()) {
        int d = a.as_value() - b.as_value();
        return d < 0 ? Order::Correct : d > 0 ? Order::Wrong : Order::Unknown;
    }
    std::vector<Packet> left = a.as_list(), right = b.as_list();
    int i = 0;
    while (true) {
        if (left.size() <= i) return right.size() > i ? Order::Correct : Order::Unknown;
        if (right.size() <= i) return Order::Wrong;
        Order c = is_ordered(left[i], right[i]);
        if (c != Order::Unknown) return c;
        ++i;
    }
};

Packet parse_item(std::string::const_iterator& begin, std::string::const_iterator end) {
    if (begin == end) throw "I didn't mean it";
    if (*begin == '[') {
        std::vector<Packet> items;
        do {
            ++begin;
            if (*begin == ']') {
                ++begin;
                return items;
            }
            items.push_back(parse_item(begin, end));
            if (*begin == ']') {
                ++begin;
                return items;
            }
        } while (*begin == ',');
        throw "nope";
    }
    auto e = begin;
    while (*e >= '0' && *e <= '9') ++e;
    if (e == begin) throw "not digit";
    int v = std::stoi(std::string(begin, e));
    begin = e;
    return v;
}

Packet parse(std::string& line) {
    auto begin = line.cbegin(), end = line.cend();
    return parse_item(begin, end);
};

int day13_1(std::vector<std::string> lines) {        
    int r = 0;
    for (int i = 0; i < lines.size(); i += 3) {
        if (is_ordered(parse(lines[i]), parse(lines[i + 1])) == Order::Correct) {
            r += (i / 3) + 1;
        }
    }
    return r;
}

int day13_2(std::vector<std::string> lines) {
    std::vector<Packet> t;
    lines.push_back("[[2]]");
    lines.push_back("[[6]]");

    for (auto& line : lines) {
        if (!line.empty()) {
            t.push_back(parse(line));
        }        
    }
    std::sort(t.begin(), t.end(), [](Packet& a, Packet& b) {
        return is_ordered(a, b) == Order::Correct;
    });

    std::map<std::string, int> text;
    
    for (int i = 0; i < t.size(); ++i) {
        text[t[i].to_string()] = i + 1;
    }

    return text["[[2]]"] * text["[[6]]"];
}

void print_cave(std::map<pt, int>& cave, pt sand) {
    int xl = sand.x;
    int yl = sand.y;
    int xh = sand.x;
    int yh = sand.y;
    for (auto [p, c] : cave) {
        xl = std::min(xl, p.x);
        xh = std::max(xh, p.x);
        yl = std::min(yl, p.y);
        yh = std::max(yh, p.y);
    }
    for (int y = yl; y <= yh; ++y) {
        for (int x = xl; x <= xh; ++x) {
            auto it = cave.find({x, y});
            if (sand == pt(x, y)) {
                std::cout << "X";
            } else if (it == cave.end()) {
                std::cout << ".";
            } else switch (it->second) {
                case 1: std::cout << "#"; break;
                case 2: std::cout << "o"; break;
                case 3: std::cout << "~"; break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int day14_1(std::vector<std::string> lines) {
    std::map<pt, int> cave;
    std::regex r("(\\d*),(\\d*)");
    int d = 0;
    for (auto& line : lines) {
        auto it = std::sregex_iterator(line.begin(), line.end(), r);
        int px = 0, py = 0;
        while (it != std::sregex_iterator()) {
            int x = std::stoi((*it)[1]);
            int y = std::stoi((*it)[2]);
            ++it;
            if (px == 0) {
                px = x; py = y;
                continue;
            }
            while (px != x || py != y) {
                cave[{px, py}] = 1;
                px += sign(x - px);
                py += sign(y - py);
            }
            cave[{px, py}] = 1;
            d = std::max(d, py + 2);
        }
    }
    
    int sand = 0;
    while (true) {
        int x = 500, y = 0;
        while (y <= d) {
            if (cave.find({x, y + 1}) == cave.end()) {
                y = y + 1;
            } else if (cave.find({x - 1, y + 1}) == cave.end()) {
                x = x - 1;
                y = y + 1;
            } else if (cave.find({x + 1, y + 1}) == cave.end()) {
                x = x + 1;
                y = y + 1;
            } else {
                cave[{x, y}] = 2;
                break;
            }
        }
        // print_cave(cave, {x, y});
        if (y >= d) {
            break;
        }
        sand += 1;
    }
    return sand;
}

int day14_2(std::vector<std::string> lines) {
    std::map<pt, int> cave;
    std::regex r("(\\d*),(\\d*)");
    int d = 0;
    for (auto& line : lines) {
        auto it = std::sregex_iterator(line.begin(), line.end(), r);
        int px = 0, py = 0;
        while (it != std::sregex_iterator()) {
            int x = std::stoi((*it)[1]);
            int y = std::stoi((*it)[2]);
            ++it;
            if (px == 0) {
                px = x; py = y;
                continue;
            }
            while (px != x || py != y) {
                cave[{px, py}] = 1;
                px += sign(x - px);
                py += sign(y - py);
            }
            cave[{px, py}] = 1;
            d = std::max(d, py + 2);
        }
    }
    for (int x = 500 - d - 1; x <= 500 + d + 1; ++x) {
        cave[{x, d}] = 3;
    }
    print_cave(cave, {500, 0});
    int sand = 0;
    while (cave[{500, 0}] == 0) {
        int x = 500, y = 0;
        while (y <= d) {
            if (cave.find({x, y + 1}) == cave.end()) {
                y = y + 1;
            } else if (cave.find({x - 1, y + 1}) == cave.end()) {
                x = x - 1;
                y = y + 1;
            } else if (cave.find({x + 1, y + 1}) == cave.end()) {
                x = x + 1;
                y = y + 1;
            } else {
                cave[{x, y}] = 2;
                break;
            }
        }
        if (y >= d) {
            break;
        }
        sand += 1;
    }
    print_cave(cave, {500, 0});
    return sand;
}

int lsq(pt p) {
    return abs(p.x) + abs(p.y);
}

int day15_1(std::vector<std::string> lines, int y) {
    std::regex r("Sensor at x=([-0-9]*), y=([-0-9]*): closest beacon is at x=([-0-9]*), y=([-0-9]*)");
    std::vector<std::pair<pt, int>> m;
    for (auto& line : lines) {
        if (line.empty()) continue;
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        pt s(it[1], it[2]), b(it[3], it[4]);
        int d = lsq(s - b);
        m.push_back({s, d});
    }
    
    std::vector<pt> ranges;
    for (auto& p : m) {
        int d = (p.second - abs(p.first.y - y));
        if (d < 0) continue;
        int left = p.first.x - d;
        int right = p.first.x + d;
        ranges.push_back({left, right});
    }
    std::sort(ranges.begin(), ranges.end());
    int result = 0;
    pt c = ranges[0];
    for(pt p : ranges) {
        if (c.y < p.x) {
            result += c.y - c.x;
            c = p;
        } else {
            c.y = std::max(c.y, p.y);
        }
    }
    return result + c.y - c.x;
}

int64_t day15_2(std::vector<std::string> lines, int w) {
    std::regex r("Sensor at x=([-0-9]*), y=([-0-9]*): closest beacon is at x=([-0-9]*), y=([-0-9]*)");
    std::vector<std::pair<pt, int>> m;
    for (auto& line : lines) {
        if (line.empty()) continue;
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        pt s(it[1], it[2]), b(it[3], it[4]);
        int d = lsq(s - b);
        m.push_back({s, d});
    }
    pt c;
    while(c.x <= w && c.y <= w) {
        bool move = false;
        for (auto [s, d] : m) {
            if (lsq(c - s) <= d) {
                int x = std::max(c.x, s.x + d - abs(s.y - c.y) + 1);
                move |= c.x != x;
                c.x = x;
            }
        }
        if (!move) {
            return static_cast<int64_t>(c.x) * 4000000 + c.y;
        }
        if (c.x > w) {
            c.x = 0;
            c.y += 1;
        }
    }
    return -1;
}

struct day16 {
    std::map<int, int> valves;
    std::vector<int> targets;
    std::vector<std::vector<int>> routes;
    size_t size;

    day16(std::vector<std::string> lines) {
        std::regex r("Valve (..) has flow rate=(\\d*); tunnels? leads? to valves? (.*)");
        std::map<std::string, int> rooms;
        std::map<int, std::set<int>> tunnels;
        
        for (auto& line : lines) {
            if (line.empty()) continue;
            auto it = *std::sregex_iterator(line.begin(), line.end(), r);
            rooms[it[1]] = 0;
        }
        
        int num = 0;
        for (auto& [k, v] : rooms) {
            v = num++;
        }
        size = rooms.size();
        
        for (auto& line : lines) {
            if (line.empty()) continue;
            auto it = *std::sregex_iterator(line.begin(), line.end(), r);
            int n = rooms[it[1]];
            valves[n] = std::stoi(it[2]);
            if (valves[n] > 0) targets.push_back(n);
            std::string t = it[3];
            for (int i = 0; i < t.size(); i += 4) {
                tunnels[n].insert(rooms[t.substr(i, 2)]);
            }
        }
        std::sort(targets.begin(), targets.end());
        for (int i = 0; i < size; ++i) {
            routes.push_back(std::vector(size, -1));
        }
        for (int i = 0; i < size; ++i) {
            routes[i][i] = 0;
            for (int t : tunnels[i]) {
                routes[i][t] = 1;
                routes[t][i] = 1;
            }
        }
        bool updated;
        do {
            updated = false;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    if (i == j) continue;
                    for (int k = 0; k < size; ++k) {
                        if (i == k || j == k) continue;
                        if (routes[i][k] != -1 && routes[k][j] != -1) {
                            int c = routes[i][k] + routes[k][j];
                            if (c < routes[i][j] || routes[i][j] == -1) {
                                routes[i][j] = c;
                                routes[j][i] = c;
                                updated = true;
                            }
                        }
                    }
                }
            }
        } while (updated);
    }

    int first_dp(int room, int time, std::set<int> open) {
        if (time == 0) return 0;
        int score = 0;
        if (room != 0) {
            time -= 1;
            open.insert(room);
        }
        for (int t : targets) {
            int utime = time - routes[room][t];
            if (!open.contains(t) && utime > 0){
                score = std::max(score, first_dp(t, utime, open));
            }
        }
        return score + valves[room] * time;
    }
    
    int first() {
        return first_dp(0, 30, {});
    }
    
    int first_bfs() {
        struct state {
            int room;
            int time;
            int score;
            std::set<int> open;
        };
        int max = 0;
        std::deque<state> bfs;
        bfs.push_back({0, 0, 0, {}});
        while (!bfs.empty()) {
            state s = bfs.front();
            bfs.pop_front();
            max = std::max(max, s.score);
            for (int t : targets) {
                if (!s.open.contains(t)) {
                    int time = s.time + routes[s.room][t] + 1;
                    int score = (30 - time) * valves[t];
                    if (score > 0) {
                        std::set<int> open(s.open);
                        open.insert(t);
                        bfs.push_back({ t, time, s.score + score, open });
                    }
                }
            }
        }
        return max;
    }
    
    std::set<std::tuple<int, int, int, std::set<int>>> seen;
    int hit = 0, test = 0;
    int second_dp(int me, int me_steps, int slon, int slon_steps, int time, std::set<int> open, bool diag = false) {
        ++test;
        if (time <= 0)
            return 0;
                
        int score = 0;
        int local = 0;
        if (me_steps == 0) {
            local += time * valves[me];
            open.insert(me);
        }
        if (slon_steps == 0) {
            local += time * valves[slon];
            open.insert(slon);
        }

        if (me_steps == 0 && slon_steps == 0) {
            for (int m : targets) {
                if (open.contains(m)) continue;
                for (int s : targets) {
                    if (s == m || open.contains(s)) continue;
                    int me_open = routes[me][m] + 1;
                    int slon_open = routes[slon][s] + 1;
                    int dt = std::min(me_open, slon_open);
                    if (time <= dt) continue;
                    if (diag) std::cout << m << " " << s << " " << hit << " " << test << " " << score << "\n";
                    score = std::max(score, second_dp(m, me_open - dt, s, slon_open - dt, time - dt, open));
                }
            }
        } else if (me_steps == 0) {
            // I'll go for a walk while slon works.
            score = second_dp(0, time, slon, 0, time - slon_steps, open);
            for (int m : targets) {
                if (open.contains(m) || m == slon) continue;
                int me_open = routes[me][m] + 1;
                int slon_open = slon_steps;
                int dt = std::min(me_open, slon_open);
                if (time <= dt) continue;
                score = std::max(score, second_dp(m, me_open - dt, slon, slon_open - dt, time - dt, open));
            }
        } else if (slon_steps == 0) {
            for (int s : targets) {
                if (open.contains(s) || s == me) continue;
                int me_open = me_steps;
                int slon_open = routes[slon][s] + 1;
                int dt = std::min(me_open, slon_open);
                if (time <= dt) continue;
                score = std::max(score, second_dp(me, me_open - dt, s, slon_open - dt, time - dt, open));
            }
        } else {
            throw "wtf";
        }
        return score + local;
    }

    
    int second() {
        return second_dp(0, 0, 0, 0, 26, {}, true);
    }
};

struct day17 {
    std::vector<std::vector<int>> floor;
    std::vector<std::vector<std::vector<int>>> shapes;
    
    day17() {
        floor = std::vector<std::vector<int>>(7);
        shapes =
        {
            { {1, 1, 1, 1} },
            {
                { 0, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 0 },
            },
            {
                { 0, 0, 1 },
                { 0, 0, 1 },
                { 1, 1, 1 },
            },
            {
                { 1 },
                { 1 },
                { 1 },
                { 1 },
            },
            {
                { 1, 1 },
                { 1, 1 },
            },
        };
    }
    
    void print(pt f, int d, std::vector<std::vector<int>>& shape) {
        int h = static_cast<int>(shape.size());
        int w = static_cast<int>(shape[0].size());
        for (int y = d; y >= 0; --y) {
            std::cout << '|';
            for (int x = 0; x < floor.size(); ++x) {
                bool cover = y <= f.y && y > f.y - h && x >= f.x && x < f.x + w;
                bool shape_full = cover && shape[f.y - y][x - f.x];
                bool rock = floor[x][y] > 0;
                if (rock) {
                    if (shape_full) std::cout << "?";
                    else std::cout << "#";
                } else if (shape_full) std::cout << "@";
                else std::cout << ".";
            }
            std::cout << "|\n";
        }
        std::cout << "+-------+\n\n";
    }

    void print(int n = 0) {
        int h = static_cast<int>(floor[0].size());
        for (int y = 0; y < n || (n == 0 && y < h); ++y) {
            std::cout << '|';
            for (int x = 0; x < floor.size(); ++x) {
                if (floor[x][h - y - 1] > 0) std::cout << "#";
                else std::cout << ".";
            }
            std::cout << "|\n";
        }
        std::cout << "+~~~~~~~+\n\n";
    }
    
    void drop(int sn, int& move, int& height, std::string& input) {
        auto& shape = shapes[sn % shapes.size()];
        int h = static_cast<int>(shape.size());
        int w = static_cast<int>(shape[0].size());
        int d = height + h + 3 - 1;
        pt f(2, d);
        int m = 1;
        for (auto& s : floor) {
            while (s.size() <= d) s.push_back(0);
        }
        do {
            int s = input[move % input.size()] == '>' ? 1 : -1;
            move += 1;
            if (f.x == 0 && s == -1) s = 0;
            if (f.x + w >= floor.size() && s == 1) s = 0;
            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    bool rock = floor[f.x + x + s][f.y - y] != 0;
                    if (shape[y][x] != 0 && rock) {
                        s = 0;
                    }
                }
            }
            f.x += s;
            m = 1;
            if (f.y - h < 0) m = 0;
            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    if (shape[y][x] != 0 && floor[f.x + x][f.y - y - m] != 0) {
                        m = 0;
                    }
                }
            }
            f.y -= m;
        } while (m > 0);
        
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                if (shape[y][x] != 0) {
                    floor[f.x + x][f.y - y] = 1;
                    height = std::max(height, f.y - y + 1);
                }
            }
        }
    }
    
    int64_t second(std::string input) {
        int move = 0;
        int sn = 0;
        int height = 0;
        /*
        std::map<std::tuple<int, int, int>, int> c;
        for (int i = 0; i < 50; ++i) {
            int pm = move, ph = height;
            for (int i = 0; i < 349 * 5; ++i) {
                drop(sn++, move, height, input);
            }
            
            std::cout
            << "dm = " << move - pm
            << ", jet = " << move % input.size()
            << ", dh = " << height - ph << std::endl;
            ++c[{move - pm, move % input.size(), height - ph}];
        }
        for (auto[k, v] : c) {
            auto[dm, jet, dh] = k;
            std::cout << "[" << dm <<"," << jet << "," << dh << "] = " << v << std::endl;
        }*/
        
        int64_t iters = 1000000000000;
        int64_t size = 349 * 5;
        iters -= size;
        int64_t tail_iters = iters % size;
        int64_t body = iters / size;
        
        int64_t head_iters = size;
        for (int i = 0; i < head_iters; ++i) {
            drop(sn++, move, height, input);
        }
        int64_t head_height = height;
        
        for (int i = 0; i < size; ++i) {
            drop(sn++, move, height, input);
        }
        int64_t body_height = height - head_height;
        
        int64_t old_height = height;
        for (int i = 0; i < tail_iters; ++i) {
            drop(sn++, move, height, input);
        }
        int64_t tail_height = height - old_height;
        // 1591977080108
        // 1591977077342
        return head_height + body * body_height + tail_height;
    }
    
    int first(std::string input, int n = 2022, int height = 0) {
        int move = 0;
        for (int i = 0; i < n; ++i) {
            auto& shape = shapes[i % shapes.size()];
            int h = static_cast<int>(shape.size());
            int w = static_cast<int>(shape[0].size());
            int d = height + h + 3 - 1;
            pt f(2, d);
            int m = 1;
            for (auto& s : floor) {
                while (s.size() <= d) s.push_back(0);
            }
            do {
                // print(f, d, shape);
                int s = input[move % input.size()] == '>' ? 1 : -1;
                move += 1;
                if (f.x == 0 && s == -1) s = 0;
                if (f.x + w >= floor.size() && s == 1) s = 0;
                for (int x = 0; x < w; ++x) {
                    for (int y = 0; y < h; ++y) {
                        bool rock = floor[f.x + x + s][f.y - y] != 0;
                        if (shape[y][x] != 0 && rock) {
                            s = 0;
                        }
                    }
                }
                f.x += s;
                m = 1;
                if (f.y - h < 0) m = 0;
                for (int x = 0; x < w; ++x) {
                    for (int y = 0; y < h; ++y) {
                        if (shape[y][x] != 0 && floor[f.x + x][f.y - y - m] != 0) {
                            m = 0;
                        }
                    }
                }
                f.y -= m;
            } while (m > 0);
            
            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    if (shape[y][x] != 0) {
                        floor[f.x + x][f.y - y] = 1;
                        height = std::max(height, f.y - y + 1);
                    }
                }
            }
            // print(f, d, shape); std::cout << "============\n";
        }

        return height;
    }
};

int day18_1(std::vector<std::string> input) {
    std::regex r("(\\d*),(\\d*),(\\d*)");

    std::set<std::vector<int>> scan;
    for (std::string& line : input) {
        if (line.empty()) continue;
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        std::vector<int> c;
        for (int i = 1; i <= 3; ++i) {
            c.push_back(std::stoi(it[i]));
        }
        scan.insert(c);
    }
    int e = 0;
    for (auto c : scan) {
        for (int i = 0; i < 3; ++i) {
            auto n = c;
            n[i] = c[i] + 1;
            if (!scan.contains(n)) e++;
            n[i] = c[i] - 1;
            if (!scan.contains(n)) e++;
        }
    }
    return e;
}

int day18_2(std::vector<std::string> input) {
    std::regex r("(\\d*),(\\d*),(\\d*)");

    std::vector<int> sizes(3);
    for (std::string& line : input) {
        if (line.empty()) continue;
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        for (int i = 0; i < 3; ++i) {
            sizes[i] = std::max(sizes[i], std::stoi(it[i + 1]) + 3);
        }
    }
    
    std::vector<std::vector<std::vector<int>>> scan(sizes[0], std::vector<std::vector<int>>(sizes[1], std::vector<int>(sizes[2])));
    for (std::string& line : input) {
        if (line.empty()) continue;
        auto it = *std::sregex_iterator(line.begin(), line.end(), r);
        std::vector<int> c;
        for (int i = 0; i < 3; ++i) {
            c.push_back(std::stoi(it[i + 1]) + 1);
        }
        scan[c[0]][c[1]][c[2]] = 1;
    }
    scan[0][0][0] = 2;
    int n = sizes[0] * sizes[1] * sizes[2];
    for (int k = 0; k < n; ++k) {
        for (int a = 0; a < scan.size(); ++a) {
            for (int b = 0; b < scan[a].size(); ++b) {
                for (int c = 0; c < scan[a][b].size(); ++c) {
                    if(scan[a][b][c] != 2) continue;
                    if(a > 0            && scan[a - 1][b][c] == 0) scan[a - 1][b][c] = 2;
                    if(a < sizes[0] - 1 && scan[a + 1][b][c] == 0) scan[a + 1][b][c] = 2;
                    if(b > 0            && scan[a][b - 1][c] == 0) scan[a][b - 1][c] = 2;
                    if(b < sizes[1] - 1 && scan[a][b + 1][c] == 0) scan[a][b + 1][c] = 2;
                    if(c > 0            && scan[a][b][c - 1] == 0) scan[a][b][c - 1] = 2;
                    if(c < sizes[2] - 1 && scan[a][b][c + 1] == 0) scan[a][b][c + 1] = 2;
                }
            }
        }
    }
    int e = 0;
    for (int a = 0; a < scan.size(); ++a) {
        for (int b = 0; b < scan[a].size(); ++b) {
            for (int c = 0; c < scan[a][b].size(); ++c) {
                if(scan[a][b][c] != 1) continue;
                if(a > 0            && scan[a - 1][b][c] == 2) ++e;
                if(a < sizes[0] - 1 && scan[a + 1][b][c] == 2) ++e;
                if(b > 0            && scan[a][b - 1][c] == 2) ++e;
                if(b < sizes[1] - 1 && scan[a][b + 1][c] == 2) ++e;
                if(c > 0            && scan[a][b][c - 1] == 2) ++e;
                if(c < sizes[2] - 1 && scan[a][b][c + 1] == 2) ++e;
            }
        }
    }

    return e;
}

int day20_1(std::vector<std::string> lines) {
    std::list<int> nums;
    using node_type = decltype(nums)::iterator;
    std::vector<node_type> nodes;
    for (std::string line : lines) {
        if (line.empty()) continue;
        nodes.push_back(nums.insert(nums.end(), std::stoi(line)));
    }
    for (auto& node : nodes) {
        node_type npos = std::next(node);
        int val = *node;
        int n = *node;
        nums.erase(node);
        while(n != 0) {
            if (n < 0) {
                if (npos == nums.begin()) {
                    npos = nums.end();
                }
                npos = std::prev(npos);
                ++n;
            } else {
                if (npos == nums.end()) {
                    npos = nums.begin();
                }
                npos = std::next(npos);
                --n;
            }
        }
        node = nums.insert(npos, val);
    }
    int a = 0, b = 0, c = 0;
    int n = 0;
    auto it = std::find(nums.begin(), nums.end(), 0);
    while(n <= 3000) {
        if (n == 1000) a = *it;
        if (n == 2000) b = *it;
        if (n == 3000) c = *it;
        if (++it == nums.end()) {
            it = nums.begin();
        }
        ++n;
    }
    return a + b + c;
}

int64_t day20_2(std::vector<std::string> lines) {
    std::list<int64_t> nums;
    using node_type = decltype(nums)::iterator;
    std::vector<node_type> nodes;
    for (std::string line : lines) {
        if (line.empty()) continue;
        nodes.push_back(nums.insert(nums.end(), 811589153 * std::stoll(line)));
    }
    for (int i = 0; i < 10; ++i) {
        int pass = 0;
        for (int j = 0; j < nodes.size(); ++j) {
            node_type npos = std::next(nodes[j]);
            int64_t val = *nodes[j];
            int64_t n = (abs(val) % (nums.size() - 1)) * sign(val);
            nums.erase(nodes[j]);
            while(n != 0) {
                if (n < 0) {
                    if (npos == nums.begin()) {
                        npos = nums.end();
                    }
                    npos = std::prev(npos);
                    ++n;
                } else {
                    if (npos == nums.end()) {
                        npos = nums.begin();
                    }
                    npos = std::next(npos);
                    --n;
                }
            }
            nodes[j] = nums.insert(npos, val);
        }
    }
    int64_t a = 0, b = 0, c = 0;
    int p = 0;
    auto it = std::find(nums.begin(), nums.end(), 0);
    while(p <= 3000) {
        if (p == 1000) a = *it;
        if (p == 2000) b = *it;
        if (p == 3000) c = *it;
        if (++it == nums.end()) {
            it = nums.begin();
        }
        ++p;
    }
    return a + b + c;
}

int main()
{
    // std::cout << day1_1(read_input("day1.txt")) << std::endl;
    // std::cout << day1_2(read_input("day1.txt")) << std::endl;
    // std::cout << day2_1(read_input("day2.txt")) << std::endl;
    // std::cout << day2_2(read_input("day2.txt")) << std::endl;
    // std::cout << day3_1(read_input("day3.txt")) << std::endl;
    // std::cout << day3_2(read_input("day3.txt")) << std::endl;
    // std::cout << day4::first(read_input("day4.txt")) << std::endl;
    // std::cout << day4::second(read_input("day4.txt")) << std::endl;
    // std::cout << day5_1(read_input("day5.txt")) << std::endl;
    // std::cout << day5_2(read_input("day5.txt")) << std::endl;
    // std::cout << "day 6.1: " << day6(read_input("day6.txt"), 4) << "\n";
    // std::cout << "day 6.2: " << day6(read_input("day6.txt"), 14) << "\n";
    // std::cout << day7().first(read_input("day7.txt")) << std::endl;
    // std::cout << day7().second(read_input("day7.txt")) << std::endl;
    // std::cout << day8_1(read_input("day8.txt")) << std::endl;
    // std::cout << day8_2(read_input("day8.txt")) << std::endl;
    // std::cout << day9::day9_1(read_input("day9.txt")) << std::endl;
    // std::cout << day9::day9_2(read_input("day9.txt")) << std::endl;
    // std::cout << day10_1(read_input("day10.txt")) << std::endl;
    // std::cout << day10_2(read_input("day10.txt")) << std::endl;
    // std::cout << day11_1() << std::endl;
    // std::cout << day11_2() << std::endl;
    // std::cout << day12_1(read_input("day12.txt")) << std::endl;
    // std::cout << day12_2(read_input("day12.txt")) << std::endl;
    // std::cout << day13_1(read_input("day13.txt")) << std::endl;
    // std::cout << day13_2(read_input("day13.txt")) << std::endl;
    // std::cout << day14_1(read_input("day14.txt")) << std::endl;
    // std::cout << day14_2(read_input("day14.txt")) << std::endl;
    // std::cout << day15_1(read_input("day15.txt"), 2000000) << std::endl;
    // std::cout << day15_2(read_input("day15.txt"), 4000000) << std::endl;
    // std::cout << day16(read_input("day16.txt")).first() << std::endl;
    // std::cout << day16(read_input("day16.txt")).first_bfs() << std::endl;
    // std::cout << day16(read_input("day16.txt")).second() << std::endl;
    // std::cout << day17().first(">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>") << std::endl;
    // std::cout << day17().first(read_input("day17.txt")[0]) << std::endl;
    // std::cout << day17().second(">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>") << std::endl;
    // std::cout << day17().second(read_input("day17.txt")[0]) << std::endl;
    // std::cout << day18_1(read_input("day18.txt")) << std::endl;
    // std::cout << day18_2(read_input("day18.txt")) << std::endl;
    std::cout << day20_1(read_input("day20.txt")) << std::endl;
    std::cout << day20_2(read_input("day20.txt")) << std::endl;
    return 0;
}
