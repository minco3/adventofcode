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
    uint64_t total = 0;
    std::unordered_set<Point> visited;
    std::stack<Point> stack;
    for (int64_t y = 0; std::getline(file, line); y++)
    {
        map.push_back(line);
        if (int64_t x = line.find("S"); x != std::string::npos)
        {
            stack.push({x, y});
        }
    }
    while (!stack.empty())
    {
        Point p = stack.top();
        stack.pop();
        visited.insert(p);
        if (p.y == map.size())
        {
            continue;
        }
        else if (map[p.y][p.x] == '^')
        {
            total++;
            for (Point next :
                 std::array<Point, 2>{{{1, 0}, {-1, 0}}} |
                     std::views::transform([&](Point d) { return p + d; }) |
                     std::views::filter([&](const Point& q)
                                        { return !visited.contains(q); }))
            {
                stack.push(next);
            }
        }
        else if (!visited.contains(p + Point(0, 1)))
        {
            stack.push(p + Point(0, 1));
        }
    }
    std::cout << total << '\n';
}