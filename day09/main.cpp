#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;

    std::vector<std::vector<int64_t>> nums;

    while (std::getline(file, str))
    {
        std::stringstream sstr(str);
        nums.emplace_back(std::istream_iterator<int64_t>(sstr),
            std::istream_iterator<int64_t>());
    }

    size_t sum = 0, psum = 0;
    for (auto& row : nums)
    {
        std::vector<std::vector<int64_t>> diffs{row};
        while (std::find_if(
                   diffs.back().begin(), diffs.back().end(),
                   [](int64_t n) { return n != 0; }) != diffs.back().end())
        {
            diffs.emplace_back(diffs.back().size()-1, 0);
            std::vector<int64_t>& current_row = *std::prev(diffs.end(), 2);
            for (size_t i = 0; i < diffs.back().size(); i++)
            {
                diffs.back().at(i) = current_row.at(i+1)-current_row.at(i);
            }
        }
        // for (auto r : diffs)
        // {
        //     for (auto n : r)
        //     {
        //         std::cout << n << ' ';
        //     }
        //     std::cout << std::endl;
        // }
        int64_t next_value = 0;
        for(auto d : diffs)
        {
            next_value += d.back();
        }
        // std::cout << next_value << std::endl;
        sum += next_value;
        int64_t prev_value = 0;
        for(auto d : diffs | std::views::reverse)
        {
            prev_value = d.front()-prev_value;
        }
        // std::cout << prev_value << std::endl;
        psum += prev_value;
    }

    std::cout << sum << ' ' << psum;
}