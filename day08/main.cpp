#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <ranges>
#include <numeric>

using pos_t = size_t;

bool done(pos_t pos) { return (pos & 0xFF) == 'Z'; }

size_t lcm(size_t a, size_t b) { return a * b / std::gcd(a, b); }

pos_t to_pos(std::string str)
{
    size_t pos = 0;
    for (char c : str)
    {
        pos <<= 8;
        pos |= c;
    }
    return pos;
}

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t sum = 0;

    std::map<pos_t, std::pair<pos_t, pos_t>> graph;

    std::string instructions;

    std::getline(file, instructions);
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::pair<pos_t, size_t>> positions;

    while (std::getline(file, str))
    {
        std::stringstream sstr(str);
        std::string name, left, right;
        sstr >> name;
        sstr.ignore(4);
        sstr >> left >> right;
        left.pop_back();
        right.pop_back();
        graph.insert({{to_pos(name), {to_pos(left), to_pos(right)}}});
        if (name.back() == 'A')
        {
            positions.push_back({to_pos(name), 0});
        }
    }

    for (auto& record : positions)
    {
        auto pos = record.first;
        size_t count = 0;
        for (size_t i = 0; !done(pos); i++, count++)
        {
            if (i == instructions.size())
            {
                i = 0;
            }

            if (instructions.at(i) == 'L')
            {
                pos = graph.at(pos).first;
            }
            else
            {
                pos = graph.at(pos).second;
            }
        }
        record.second = count+1;
    }
    std::cout << std::ranges::fold_right_last(positions | std::ranges::views::values, lcm).value();
}