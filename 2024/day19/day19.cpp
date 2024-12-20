#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    constexpr bool debug = false;

    std::vector<std::string> colors;
    std::vector<std::string> towels;

    std::getline(file, str);
    std::stringstream sstr(str);
    while (std::getline(sstr, str, ','))
    {
        if (str[0] == ' ')
            str = str.substr(1);
        towels.push_back(str);
    }

    std::getline(file, str);

    while (std::getline(file, str))
    {
        colors.push_back(str);
    }

    std::unordered_map<std::string_view, size_t> cache = {{"", 1}};

    std::function<size_t(std::string_view)> recurse =
        [&](std::string_view color) -> size_t
    {
        if (cache.contains(color))
        {
            return cache[color];
        }

        size_t count = 0;
        for (auto& towel : towels)
        {
            if (color.substr(0, towel.size()) == towel)
            {
                if (size_t n = recurse(color.substr(towel.size())))
                {
                    count += n;
                }
            }
        }
        
        cache[color] = count;
        return count;
    };

    for (auto& color : colors)
    {
        if (size_t n = recurse(color))
        {
            count++;
            count2 += n;
            if (debug)
                std::cout << color << ' ' << n << '\n';
        }
    }

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}