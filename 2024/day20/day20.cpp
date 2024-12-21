#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct point
{
    int64_t x, y;
    point operator+(point r) const { return {x + r.x, y + r.y}; }
    point operator-() const { return {-x, -y}; }
    point operator-(point r) const { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point& p)
    {
        return {k * p.x, k * p.y};
    }
    bool operator==(point r) const { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};
constexpr point left = {-1, 0};
constexpr point right = {1, 0};
constexpr point up = {0, -1};
constexpr point down = {0, 1};

template <>
struct std::hash<point>
{
    std::size_t operator()(point const& s) const noexcept
    {
        std::size_t h1 = std::hash<int64_t>{}(s.x);
        std::size_t h2 = std::hash<int64_t>{}(s.y);
        return h1 ^ (h2 << 1);
    }
};

template <>
struct std::formatter<point> : std::formatter<std::string>
{
    template <class FormatContext>
    auto format(point p, FormatContext& ctx) const
    {
        return formatter<string>::format(
            std::format("[{}, {}]", p.x, p.y), ctx);
    }
};

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

int64_t distance(point a, point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    std::vector<std::vector<bool>> map;
    point start, end;
    constexpr bool debug = false;

    while (std::getline(file, str))
    {
        size_t i = str.find('S');
        if (i != std::string::npos)
        {
            start = {static_cast<int64_t>(i), static_cast<int64_t>(map.size())};
        }
        i = str.find('E');
        if (i != std::string::npos)
        {
            end = {static_cast<int64_t>(i), static_cast<int64_t>(map.size())};
        }
        std::vector<bool> line;
        for (auto c : str)
        {
            line.push_back(c == '#' ? true : false);
        }
        map.push_back(line);
    }

    std::vector<std::vector<int64_t>> distances(
        map.size(), std::vector<int64_t>(map[0].size(), 0));

    auto adv = [&](point& p)
    {
        for (auto d : {up, down, left, right})
        {
            point p2 = p + d;
            if (contains(p2, map[0].size(), map.size()) && !map[p2.y][p2.x] &&
                (!distances[p2.y][p2.x] ||
                 distances[p.y][p.x] < distances[p2.y][p2.x]))
            {
                p = p2;
                return;
            }
        }
    };
    size_t i = 1;
    for (point p = start; p != end; adv(p))
    {
        distances[p.y][p.x] = i++;
    }
    distances[end.y][end.x] = i;

    if constexpr (debug)
    {
        for (size_t i = 0; i < map.size(); i++)
        {
            for (size_t j = 0; j < map[0].size(); j++)
            {
                std::print("{:4}", distances[i][j]);
            }
            std::cout << '\n';
        }
    }

    auto find_cheats = [&](int64_t max_distance) -> size_t
    {
        size_t count = 0;
        for (point p = start; p != end; adv(p))
        {
            for (int64_t y = -max_distance; y <= max_distance; y++)
            {
                for (int64_t x = -max_distance + std::abs(y);
                     x <= max_distance - std::abs(y); x++)
                {
                    auto p2 = p + point(x, y);
                    if (!contains(p2, map[0].size(), map.size()) ||
                        map[p2.y][p2.x])
                    {
                        continue;
                    }
                    else if (int64_t saved_time = distances[p2.y][p2.x] -
                                                  distances[p.y][p.x] -
                                                  distance(p, p2);
                             saved_time >= 100)
                    {
                        count++;
                    }
                }
            }
        }
        return count;
    };

    count = find_cheats(2);
    count2 = find_cheats(20);

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}