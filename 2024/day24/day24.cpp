#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    constexpr bool debug = false;

    uint64_t x = 0, y = 0, z = 0;

    while (std::getline(file, str) && !str.empty())
    {
        (str[0] == 'x' ? x : y) |= static_cast<uint64_t>(str[5] - '0')
                                   << std::stoi(str.substr(1, 2));
    }
    if constexpr (debug)
        std::println("{} {}", x, y);

    std::unordered_map<
        std::string, std::tuple<std::string, std::string, std::string>>
        ops;

    while (std::getline(file, str))
    {
        const std::regex re(R"((.*) (.*) (.*) -> (.*))");
        std::smatch match;
        if (std::regex_match(str, match, re))
        {
            ops[match[4]] = {match[1], match[3], match[2]};
        }
    }

    std::unordered_map<std::string, bool> cache;
    std::function<bool(std::string)> get = [&](std::string id) -> bool
    {
        if (cache.contains(id))
        {
            return cache[id];
        }
        bool result;
        auto [lhs, rhs, op] = ops[id];
        if (id[0] == 'x')
        {
            result = x & 1ull << std::stoull(id.substr(1, 2));
        }
        else if (id[0] == 'y')
        {
            result = y & 1ull << std::stoull(id.substr(1, 2));
        }
        else
        {
            bool l = get(lhs), r = get(rhs);
            if (op == "AND")
            {
                result = l && r;
            }
            else if (op == "OR")
            {
                result = l || r;
            }
            else if (op == "XOR")
            {
                result = l ^ r;
            }
        }
        cache[id] = result;
        return result;
    };

    for (uint64_t i = 0; i < 64; i++)
    {
        std::string id = std::format("z{:02}", i);
        if (ops.contains(id))
        {
            uint64_t result = get(id);
            if constexpr (debug)
            {
                std::println("{} {:b} {}", id, result, result << i);
            }
            z |= result << i;
        }
    }

    if constexpr (debug)
    {
        std::println("{}", cache);
    }

    std::println(
        "{}, {}", z,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}