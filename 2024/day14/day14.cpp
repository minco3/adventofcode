#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <print>
#include <regex>
#include <vector>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point& p)
    {
        return {k * p.x, k * p.y};
    }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

bool is_int(double v) { return std::abs(v - std::round(v)) < 0.001; }

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;

    constexpr const int64_t width = 101, height = 103;
    // constexpr const size_t width = 101, height = 103;

    std::vector<std::pair<point, point>> robots;
    std::vector<std::vector<size_t>> map(height, std::vector<size_t>(width, 0));

    while (std::getline(file, str))
    {
        const std::regex re(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
        std::smatch match;
        if (std::regex_match(str, match, re))
        {
            auto robot = std::make_pair<point, point>(
                {std::stoll(match[1]), std::stoll(match[2])},
                {std::stoll(match[3]), std::stoll(match[4])});
            robots.push_back(robot);
            auto pos = robot.first + (100 * robot.second);
            map[((pos.y % height) + height) % height]
               [((pos.x % width) + width) % width]++; // positive modulo
            // std::println(
            //     "{}, {}, {}, {}", robots.back().first.x,
            //     robots.back().first.y, robots.back().second.x,
            //     robots.back().second.y);
        }
    }

    std::array<std::array<size_t, 2>, 2> quads = {{{0, 0}, {0, 0}}};
    size_t y = 0;
    for (size_t i = 0; i < height; i++)
    {
        size_t x = 0;
        if (i == height / 2)
        {
            y++;
            continue;
        }
        for (size_t j = 0; j < width; j++)
        {
            if (j == width / 2)
            {
                x++;
                continue;
            }
            quads[y][x] += map[i][j];
            // std::cout << (map[i][j] ? std::to_string(map[i][j]) : ".");
        }
        // std::cout << '\n';
    }

    count = 1;

    for (auto [x, y] : quads)
    {
        count *= x * y;
        // std::cout << x  << ' ' << y << '\n';
    }

    size_t i = 0;

    std::fstream outfile(SOURCE_DIR "/out.txt", std::ios::out);
    while (i < 10000)
    {
        std::vector<std::vector<size_t>> map(
            height, std::vector<size_t>(width, 0));

        for (auto robot : robots)
        {
            auto pos = robot.first + (i * robot.second);
            map[((pos.y % height) + height) % height]
               [((pos.x % width) + width) % width]++; // positive modulo
        }

        bool tree = false;
        for (size_t i = 0; i < height - 2; i++)
        {
            for (size_t j = 0; j < width - 2; j++)
            {
                size_t count = 0;
                for (size_t k = 0; k < 3; k++)
                {
                    for (size_t l = 0; l < 3; l++)
                    {
                        if (map[i + k][j + l])
                        {
                            count++;
                        }
                    }
                }
                if (count == 9)
                {
                    tree = true;
                }
            }
        }

        if (tree)
        {
            count2 = i;
            outfile << i << '\n';
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < width; j++)
                {
                    outfile << (map[i][j] ? "█" : " ");
                }
                outfile << '\n';
            }
        }
        i++;
    }

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}