#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <set>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <cassert>
#include <array>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point &p) { return {k * p.x, k * p.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

size_t dfs(point p, std::vector<std::string> &map, std::set<point> &s)
{
    size_t rating = 0;
    if (map[p.y][p.x] == '9')
    {
        s.insert(p);
        return 1;
    }

    std::array<point, 4> dirs = {{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}}; // up, down, left, or right
    for (auto d : dirs)
    {
        auto p2 = p + d;
        if (contains(p2, map[0].size(), map.size()) && map[p2.y][p2.x] - map[p.y][p.x] == 1)
        {
            rating += dfs(p2, map, s);
        }
    }
    return rating;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    std::vector<size_t> ids;
    std::vector<std::string> map;
    std::vector<point> trailheads;

    while (std::getline(file, str))
    {
        size_t i = str.find('0');
        while (i != str.npos)
        {
            trailheads.emplace_back(i, map.size());
            i = str.find('0', i + 1);
        }
        map.push_back(str);
    }

    for (auto trailhead : trailheads)
    {
        std::set<point> points;
        count2 += dfs(trailhead, map, points);
        count += points.size();
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}