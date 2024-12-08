#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <functional>
#include <vector>
#include <iterator>
#include <cmath>
#include <thread>
#include <atomic>
#include <iostream>
#include <cassert>
#include "tracy/public/tracy/Tracy.hpp"

int main()
{
    ZoneScoped;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    std::atomic<size_t> count = 0, count2 = 0;
    std::vector<std::thread> threads;
    while (std::getline(file, str))
    {
        threads.emplace_back([str, &count, &count2]
                             {
                                ZoneScoped;
                                size_t i = str.find(':');
                                uint64_t ans = std::stoull(str.substr(0, i));
                                std::stringstream sstr(str.substr(i + 1));
                                std::vector<uint64_t> vec = {std::istream_iterator<uint64_t>(sstr), std::istream_iterator<uint64_t>()};

                                const std::function<bool(uint64_t, size_t, bool)> solve = [&](uint64_t cur, size_t i, bool part_2) -> bool
                                {
                                    if (i == vec.size())
                                        return cur == ans;
                                    if (cur > ans)
                                        return false;
                                    return solve(cur + vec[i], i + 1, part_2) || solve(cur * vec[i], i + 1, part_2) || part_2 && solve(cur * std::pow(10, std::floor(std::log10(vec[i]) + 1)) + vec[i], i + 1, part_2);
                                };

                                count += solve(vec[0], 1, false) ? ans : 0; 
                                count2 += solve(vec[0], 1, true) ? ans : 0; });
    }

    std::println("dispatched in {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));

    for (auto &t : threads)
    {
        t.join();
    }

    std::println("{}, {}, {}", count.load(), count2.load(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
    assert(count == 28730327770375 && count2 == 424977609625985);
}