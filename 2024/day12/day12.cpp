#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <unordered_map>
#include <array>
#include <iostream>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point &p) { return {k * p.x, k * p.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

struct dfs_record
{
    size_t area, perimeter, corners;
    dfs_record operator+(dfs_record r) { return {area + r.area, perimeter + r.perimeter, corners + r.corners}; }
};

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

dfs_record dfs(point p, std::vector<std::string> &map, std::vector<std::vector<uint8_t>> &visited)
{

    if (visited[p.y][p.x])
        return {0, 0, 0};
    else
        visited[p.y][p.x] = 1;

    dfs_record record = {1, 0, 0};

    auto contains_and_equal = [&](point p2) -> bool
    { return contains(p2, map[0].size(), map.size()) && map[p2.y][p2.x] == map[p.y][p.x]; };
    auto contains_and_not_equal = [&](point p2) -> bool
    { return contains(p2, map[0].size(), map.size()) && map[p2.y][p2.x] != map[p.y][p.x]; };

    std::array<point, 4> diags = {{{1, -1}, {1, 1}, {-1, 1}, {-1, -1}}}; // up-right, down-right, down-left, or up-left
    std::array<point, 4> dirs = {{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}};    // up, down, left, or right

    for (auto d : diags)
    {
        auto p2 = p + point(d.x, 0);
        auto p3 = p + point(0, d.y);
        if ((contains_and_not_equal(p + d) && contains_and_equal(p2) && contains_and_equal(p3)) // inner corner
            || (!contains_and_equal(p2) && !contains_and_equal(p3)))                            // outer corner
        {
            record.corners++;
        }
    }

    for (auto d : dirs)
    {
        auto p2 = p + d;
        if (contains_and_equal(p2))
        {
            record = record + dfs(p2, map, visited);
        }
        else
        {
            record.perimeter++;
        }
    }
    return record;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;

    std::vector<std::string> map;
    std::vector<std::vector<uint8_t>> visited;

    while (std::getline(file, str))
    {
        map.push_back(str);
    }

    visited.resize(map.size(), std::vector<uint8_t>(map[0].size(), 0));

    for (int64_t i = 0; i < map.size(); i++)
    {
        for (int64_t j = 0; j < map[0].size(); j++)
        {
            if (visited[i][j])
                continue;

            auto [area, perimeter, edges] = dfs({j, i}, map, visited);
            count += area * perimeter;
            count2 += area * edges;
        }
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}