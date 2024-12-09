#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <set>
#include <unordered_map>
#include <iostream>
#include <utility>

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

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/sample.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    std::unordered_map<char, std::vector<point>> frequencies;
    std::vector<std::string> map;
    int64_t y = 0;
    while (std::getline(file, str))
    {
        for (int64_t x = 0; x < str.size(); x++)
        {
            if (str[x] != '.')
            {
                frequencies[str[x]].emplace_back(x, y);
            }
        }
        map.push_back(str);
        y++;
    }
    int64_t height = y, width = str.size();

    std::set<point> antinodes;

    for (auto [frequency, antennas] : frequencies)
    {
        for (size_t i = 0; i < antennas.size(); i++)
        {
            for (size_t j = i + 1; j < antennas.size(); j++)
            {
                auto p1 = antennas[i], p2 = antennas[j];
                for (auto p : {p1 + (p1 - p2), p2 + (p2 - p1)})
                {
                    if (contains(p, width, height))
                    {
                        antinodes.insert(p);
                    }
                }
            }
        }
    }
    count = antinodes.size();

    antinodes.clear();
    for (auto [frequency, antennas] : frequencies)
    {
        for (size_t i = 0; i < antennas.size(); i++)
        {
            for (size_t j = i + 1; j < antennas.size(); j++)
            {
                auto p1 = antennas[i], p2 = antennas[j];
                auto solve = [&](point p, point d)
                {
                    int64_t k = 0;
                    while (contains(p + k * d, width, height))
                    {
                        antinodes.insert(p + k * d);
                        k++;
                    };
                };

                solve(p1, p1 - p2);
                solve(p2, p2 - p1);
            }
        }
    }
    count2 = antinodes.size();

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}