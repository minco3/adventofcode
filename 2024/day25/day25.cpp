#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <set>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    constexpr bool debug = false;

    uint64_t count = 0;

    std::vector<std::array<uint8_t, 5>> keys;
    std::vector<std::array<uint8_t, 5>> locks;

    std::array<std::string, 5> map;
    bool key = false, first_line = true;

    auto push_pins = [&](const std::array<std::string, 5>& map, bool key)
    {
        std::array<uint8_t, 5> pins;
        for (size_t i = 0; i < pins.size(); i++)
        {
            pins[i] = map[i].find((key ? '#' : '.')) - 1;
            if (key)
            {
                pins[i] = 5 - pins[i];
            }
        }
        (key ? keys : locks).push_back(pins);
    };

    while (std::getline(file, str))
    {
        if (first_line)
        {
            first_line = false;
            key = str == ".....";
        }

        for (size_t i = 0; i < str.size(); i++)
        {
            map[i] += str[i];
        }

        if (str.empty())
        {
            push_pins(map, key);
            first_line = true;
            for (auto& l : map)
            {
                l.clear();
            }
        }
    }
    push_pins(map, key);

    if constexpr (debug)
    {
        std::println("{}\n{}", keys, locks);
    }

    for (auto key : keys)
    {
        for (auto lock : locks)
        {
            bool collision = false;
            for (auto [l, r] : std::views::zip(key, lock))
            {
                if (l + r > 5)
                {
                    collision = true;
                    break;
                }
            }
            if (!collision)
            {
                count++;
            }
        }
    }

    std::println(
        "{}, {}", count,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}