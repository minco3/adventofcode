#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct point2
{
    int64_t x, y;
};

int main()
{
    std::fstream file("input.txt");
    std::string line;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::vector<point2> points;
    int64_t max = 0;
    while (std::getline(file, line))
    {
        int64_t x, y;
        std::sscanf(line.c_str(), "%llu,%llu", &x, &y);
        points.push_back({x, y});
    }
    for (uint32_t i = 0; i < points.size(); i++)
    {
        for (uint32_t j = i + 1; j < points.size(); j++)
        {
            max = std::max(
                max, (std::abs(points[i].x - points[j].x) + 1)
                         * (std::abs(points[i].y - points[j].y) + 1)
            );
        }
    }

    std::println(
        "result: {}, time: {}", max,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );
}