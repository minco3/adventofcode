#include <chrono>
#include <fstream>
#include <iostream>
#include <print>
#include <unordered_set>
#include <vector>

struct point
{
    int64_t x, y;
    point operator+(point r) const { return {x + r.x, y + r.y}; }
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

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    std::vector<std::string> map, map2;
    std::string moves;
    point pos, pos2;
    constexpr bool debug = false;

    while (std::getline(file, str) && !str.empty())
    {
        size_t i = str.find('@');
        if (i != std::string::npos)
        {
            pos = {static_cast<int64_t>(i), static_cast<int64_t>(map.size())};
            pos2 = {
                static_cast<int64_t>(i * 2), static_cast<int64_t>(map.size())};
        }
        map.push_back(str);
        std::string str2;
        std::ranges::for_each(
            str,
            [&](char c)
            {
                switch (c)
                {
                case '@':
                    str2 += "@.";
                    break;
                case 'O':
                    str2 += "[]";
                    break;
                default:
                    str2 += std::string(2, c);
                    break;
                }
            });
        map2.push_back(str2);
    }
    while (std::getline(file, str))
    {
        moves += str;
    }

    auto chartod = [](char c) -> point
    {
        switch (c)
        {
        case '^':
            return up;
        case '>':
            return right;
        case 'v':
            return down;
        case '<':
            return left;
        default:
            return {};
        }
    };

    auto move = [](point pos, point d, std::vector<std::string>& map,
                   bool part2) -> point
    {
        if (!part2)
        {
            for (int64_t i = 1;
                 contains(pos + i * d, map[0].size(), map.size()); i++)
            {
                switch (map[(pos + i * d).y][(pos + i * d).x])
                {
                case '#':
                    return pos;
                case 'O':
                    continue;
                case '.':
                    map[pos.y][pos.x] = '.';
                    map[(pos + d).y][(pos + d).x] = '@';
                    if (i > 1)
                        map[(pos + i * d).y][(pos + i * d).x] = 'O';
                    pos = pos + d;
                    return pos;
                }
            }
        }
        else
        {
            std::unordered_set<point> pushable;
            std::vector<point> boxes;
            boxes.push_back(pos + d);
            while (!boxes.empty())
            {
                auto p = boxes.back();
                boxes.pop_back();
                switch (map[p.y][p.x])
                {
                case '#':
                    return pos;
                case '[':
                    pushable.insert(p);
                    if (d == up || d == down)
                    {
                        boxes.push_back(p + d);
                        boxes.push_back(p + d + right);
                    }
                    else
                    {
                        boxes.push_back(p + 2 * d);
                    }
                    break;
                case ']':
                    pushable.insert(p + left);
                    if (d == up || d == down)
                    {
                        boxes.push_back(p + d + left);
                        boxes.push_back(p + d);
                    }
                    else
                    {
                        boxes.push_back(p + 2 * d);
                    }
                    continue;
                case '.':
                    continue;
                }
            }
            for (auto p : pushable)
            {
                map[p.y][p.x] = '.';
                map[p.y][p.x + 1] = '.';
            }
            map[pos.y][pos.x] = '.';
            pos = pos + d;
            map[pos.y][pos.x] = '@';
            for (auto p : pushable)
            {
                map[(p + d).y][(p + d).x] = '[';
                map[(p + d).y][(p + d).x + 1] = ']';
            }
        }
        return pos;
    };

    for (char c : moves)
    {
        if (debug)
            std::cout << c << '\n';

        auto d = chartod(c);
        pos = move(pos, d, map, false);
        pos2 = move(pos2, d, map2, true);

        if (debug)
        {
            for (auto& line : map2)
            {
                std::cout << line << '\n';
            }
        }
    }

    auto count_boxes = [](std::vector<std::string>& map) -> size_t
    {
        size_t count = 0;
        for (size_t i = 0; i < map.size(); i++)
        {
            for (size_t j = 0; j < map[0].size(); j++)
            {
                if (map[i][j] == '[' || map[i][j] == 'O')
                {
                    count += i * 100 + j;
                }
            }
        }
        return count;
    };

    count = count_boxes(map);
    count2 = count_boxes(map2);

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}