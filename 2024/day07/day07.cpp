#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <functional>
#include <vector>
#include <iterator>
#include <cmath>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    while (std::getline(file, str))
    {
        size_t i = str.find(':');
        uint64_t ans = std::stoull(str.substr(0, i));
        std::stringstream sstr(str.substr(i + 1));
        std::vector<uint64_t> vec = {std::istream_iterator<uint64_t>(sstr), std::istream_iterator<uint64_t>()};

        const std::function<bool(uint64_t, size_t)> solve = [&](uint64_t cur, size_t i) -> bool
        {
            if (i == vec.size())
                return cur == ans;
            if (cur > ans)
                return false;
            return solve(cur + vec[i], i + 1) || solve(cur * vec[i], i + 1);
        };
        count += solve(vec[0], 1) ? ans : 0;

        const std::function<bool(uint64_t, size_t)> solve2 = [&](uint64_t cur, size_t i) -> bool
        {
            if (i == vec.size())
                return cur == ans;
            if (cur > ans)
                return false;
            return solve2(cur + vec[i], i + 1) || solve2(cur * vec[i], i + 1) || solve2(cur * std::pow(10, std::floor(std::log10(vec[i]) + 1)) + vec[i], i + 1);
        };
        count2 += solve2(vec[0], 1) ? ans : 0;
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}