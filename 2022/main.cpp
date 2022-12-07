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
            if (left.left >= right.left && left.left <= right.right
                || left.right >= right.left && left.right <= right.right) c++;
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
    std::cout << day7().first(read_input("day7.txt")) << std::endl;
    std::cout << day7().second(read_input("day7.txt")) << std::endl;
    return 0;
}
