#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using pos_t = std::string;

size_t lcm(size_t a, size_t b) { return a * b / std::gcd(a, b); }

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
        std::string name = str.substr(0, 3), left = str.substr(7, 3),
                    right = str.substr(12, 3);
        graph.emplace(name, std::pair<pos_t, pos_t>(left, right));
        if (name.back() == 'A')
        {
            positions.emplace_back(name, 0);
        }
    }

    for (auto& [pos, distance] : positions)
    {
        size_t count = 0;
        for (size_t i = 0; pos.back() != 'Z'; i++, count++)
        {
            if (i == instructions.size())
            {
                i = 0;
            }
            pos = instructions.at(i) == 'L' ? pos = graph.at(pos).first
                                            : pos = graph.at(pos).second;
        }
        distance = count + 1;
    }
    std::cout << std::ranges::fold_right_last(
                     positions | std::ranges::views::values, lcm)
                     .value();
}