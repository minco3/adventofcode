#include <algorithm>
#include <chrono>
#include <fstream>
#include <print>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    constexpr bool debug = false;

    auto mix = [](uint64_t lhs, uint64_t rhs) -> uint64_t { return lhs ^ rhs; };
    auto prune = [](uint64_t n) -> uint64_t
    { return n % 16777216; }; // assuming this gets optimized
    auto evolve = [mix, prune](uint64_t n) -> uint64_t
    {
        static std::unordered_map<uint64_t, uint64_t> cache;
        if (cache.contains(n))
        {
            return cache[n];
        }
        uint64_t n2 = n;
        n2 = prune(mix(n2, 64 * n2));
        n2 = prune(mix(n2, n2 / 32));
        n2 = prune(mix(n2, n2 * 2048));
        cache[n] = n2;
        return n2;
    };

    std::unordered_map<uint64_t, uint64_t> patterns;

    while (std::getline(file, str))
    {
        std::unordered_set<uint64_t> seen_patterns;
        uint64_t n = std::stoull(str);
        uint32_t pattern = 0;
        uint64_t price;
        for (size_t i = 0; i < 2000; i++)
        {
            n = evolve(n);
            int8_t delta = n % 10 - price;
            price = n % 10;
            pattern <<= 8;
            pattern |= *reinterpret_cast<uint8_t*>(&delta);
            if constexpr (debug)
            {
                std::println(
                    "{}, {}: {}, ({}), {:#010x}", i, n, price, delta, pattern);
            }
            if (i > 3 && !seen_patterns.contains(pattern))
            {
                seen_patterns.insert(pattern);
                patterns[pattern] += price;
            }
        }
        count += n;
    }
    if constexpr (debug)
    {
        std::println("{}", patterns);
    }
    count2 = std::ranges::max_element(
                 patterns, {}, [](const auto& pair) { return pair.second; })
                 ->second;
    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}