#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

struct Point
{
    int64_t x, y;
    Point operator+(Point rhs) { return {x + rhs.x, y + rhs.y}; }
    bool operator==(const Point&) const = default;
};

template <>
struct std::hash<Point>
{
    size_t operator()(const Point& p) const noexcept
    {
        size_t h1 = std::hash<int64_t>{}(p.x);
        size_t h2 = std::hash<int64_t>{}(p.y);
        return h1 ^ (h2 << 1);
    }
};

int main()
{
    std::fstream file("input.txt");
    std::string line;
    std::vector<std::string> map;
    std::vector<std::vector<uint64_t>> dp;
    uint64_t total = 0;
    Point start;
    for (int64_t y = 0; std::getline(file, line); y++)
    {
        map.push_back(line);
        if (int64_t x = line.find("S"); x != std::string::npos)
        {
            start = {x, y};
        }
    }
    dp.resize(map.size(), std::vector<uint64_t>(map.front().size(), 0));
    for (int64_t x = 0; x < dp.back().size(); x++)
    {
        dp.back()[x] = 1;
    }
    for (int64_t y = dp.size() - 2; y >= 0; y--)
    {
        for (int64_t x = 0; x < dp[y].size(); x++)
        {
            switch (map[y][x])
            {
            case '^':
                dp[y][x] = dp[y + 1][x - 1] + dp[y + 1][x + 1];
                break;
            default:
                dp[y][x] = dp[y + 1][x];
                break;
            }
        }
    }
    std::cout << dp[start.y][start.x] << '\n';
}