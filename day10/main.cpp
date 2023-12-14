#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
};

constexpr point left = {-1, 0};
constexpr point right = {1, 0};
constexpr point up = {0, -1};
constexpr point down = {0, 1};

const std::vector<char> tiles = {'|', '-', 'L', 'J', '7', 'F', '.'};
const std::vector<std::array<point, 2>> connections = {
    {{up, down}},   {{right, left}}, {{up, right}},     {{up, left}},
    {{left, down}}, {{right, down}}, {{{0, 0}, {0, 0}}}};

bool connects(point p, char c, point n)
{
    // p->n true
    std::vector<char>::const_iterator tile =
        std::find(tiles.begin(), tiles.end(), c);
    assert(tile != tiles.end());
    size_t tile_id = std::distance(tiles.begin(), tile);
    const std::array<point, 2>& connect = connections.at(tile_id);
    for (point cn : connect)
    {
        if (p + cn == n)
        {
            return true;
        }
    }
    return false;
}

point next(point cur, char c, point prev)
{
    std::vector<char>::const_iterator tile =
        std::find(tiles.begin(), tiles.end(), c);
    assert(tile != tiles.end());
    size_t tile_id = std::distance(tiles.begin(), tile);
    const std::array<point, 2>& connect = connections.at(tile_id);
    for (point cn : connect)
    {
        if (cur + cn != prev)
        {
            return cur + cn;
        }
    }
    return cur;
}

int main()
{
    std::fstream file(SOURCE_DIR "/test3.txt");
    std::string str;

    std::vector<std::string> map;
    point starting_pos;

    int64_t y = 0;
    while (std::getline(file, str))
    {
        map.push_back(str);
        if (int64_t x = str.find('S'); x != std::string::npos)
        {
            starting_pos = {x, y};
        }
        y++;
    }

    std::vector<std::vector<char>> visited(
        map.size(), std::vector<char>(map.at(0).size(), 0));
    visited[starting_pos.y][starting_pos.x] = 1;

    point p;
    for (point cn : {up, left, down, right})
    {
        point n = starting_pos + cn;
        if (n.y < 0 || n.y >= map.size() || n.x < 0 || n.x >= map.at(0).size())
            continue;

        if (connects(n, map[n.y][n.x], starting_pos))
        {
            p = n;
            break;
        }
    }

    size_t length = 1;

    point prev = starting_pos;
    while (p != starting_pos)
    {
        point swap = p;
        visited[p.y][p.x] = 1;
        p = next(p, map[p.y][p.x], prev);
        prev = swap;
        length++;
    }

    for (int64_t y = 0; y < visited.size(); y++)
    {
        for (int64_t x = 0; x < visited.at(y).size(); x++)
        {
            if (visited[y][x])
                continue;

            for (auto d : {up, left, down, right})
            {
                point dp = {x+d.x,y+d.y};
                if (dp.y < 0 || dp.y >= visited.size() || dp.x < 0 || dp.x >= visited.at(y).size())
                {
                    visited[y][x] = 2;
                    break;
                }

                if (visited[dp.y][dp.x] != 0 && visited[dp.y][dp.x] != 1)
                {
                    visited[y][x] = visited[dp.y][dp.x];
                    break;
                }
            }
            if (visited[y][x])
                continue;

            
        }
    }
    for (auto l : visited)
    {
        for (auto c : l)
        {
            std::cout << static_cast<uint16_t>(c);
        }
        std::cout << std::endl;
    }

    std::cout << length / 2 << std::endl;
}