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

constexpr size_t N = 1'000'000;

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
};

int main()
{
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;

    std::vector<std::string> map;

    size_t sum = 0;
    std::vector<point> galaxies;

    std::vector<bool> cols;

    std::vector<size_t> empty_cols, empty_rows;

    {
        size_t y = 0;
        while (std::getline(file, str))
        {
            cols.resize(str.size());
            map.push_back(str);
            if (str.find('#') == std::string::npos)
            {
                // part1
                // map.push_back(str);
                empty_rows.push_back(y);
            }
            for (size_t i = 0; i < str.size(); i++)
            {
                if (str.at(i) == '#')
                {
                    cols.at(i) = true;
                }
            }
            y++;
        }
    }

    for (size_t x = 0; x < cols.size(); x++)
    {
        if (!cols.at(x))
        {
            empty_cols.push_back(x);
        }
    }

    // part1
    // size_t modified = 0;
    // for (size_t i = 0; i < str.size(); i++)
    // {
    //     if (!cols.at(i))
    //     {
    //         for (auto& l : map)
    //         {
    //             l.insert(l.begin() + i + modified, '.');
    //         }
    //         modified++;
    //     }
    // }

    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map.at(y).size(); x++)
        {
            if (map[y][x] == '#')
            {
                galaxies.emplace_back(x, y);
            }
        }
    }

    for (size_t i = 0; i < galaxies.size() - 1; i++)
    {
        for (size_t j = i + 1; j < galaxies.size(); j++)
        {
            size_t dist =
                abs(galaxies.at(i).x - galaxies.at(j).x) +
                abs(galaxies.at(i).y - galaxies.at(j).y) +
                (std::count_if(
                     empty_cols.begin(), empty_cols.end(),
                     [=](size_t num)
                     {
                         return num > std::min(
                                          galaxies.at(i).x, galaxies.at(j).x) &&
                                num < std::max(
                                          galaxies.at(i).x, galaxies.at(j).x);
                     }) +
                 std::count_if(
                     empty_rows.begin(), empty_rows.end(),
                     [=](size_t num)
                     {
                         return num > std::min(
                                          galaxies.at(i).y, galaxies.at(j).y) &&
                                num < std::max(
                                          galaxies.at(i).y, galaxies.at(j).y);
                     })) *
                    (N - 1);

            // std::cout << i << ' ' << j << ' ' << dist << ' ' << count <<
            // '\n';

            sum += dist;
        }
    }

    std::cout << sum << std::endl;
}