#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
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

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

size_t distance(point a, point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    std::vector<std::string> map;
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
        map.push_back(str);
    }

    auto h = [end](point p) -> size_t
    {
        if (p.x == end.x)
        {
            return std::abs(p.x - end.x);
        }
        else if (p.y == end.y)
        {
            return std::abs(p.y - end.y);
        }
        else
        {
            return distance(p, end) + 1000;
        }
    };

    auto dtochar = [](point p) -> char
    {
        if (p == up)
            return '^';
        if (p == right)
            return '>';
        if (p == down)
            return 'v';
        if (p == left)
            return '<';
        else
            return '.';
    };

    std::vector<std::tuple<size_t, size_t, point, point, std::vector<point>>>
        open; // fscore, gscore, pos, d
    std::unordered_map<point, std::tuple<size_t, size_t, point>>
        closed; // pos -> fscore, gscore,
                // d
    std::vector<std::vector<point>> routes;

    auto print_map = [&]() -> void
    {
        auto map2 = std::vector<std::string>(map);
        for (auto [fscore, gscore, p, d, route] : open)
        {
            map2[p.y][p.x] = dtochar(d);
        }
        for (auto [p, pair] : closed)
        {
            auto [fscore, gscore, d] = pair;
            map2[p.y][p.x] = dtochar(d);
        }
        for (auto& line : map2)
        {
            std::cout << line << '\n';
        }
    };

    auto print_route = [&](std::vector<point> route)
    {
        if (debug)
        {
            for (int64_t i = 0; i < map.size(); i++)
            {
                for (int64_t j = 0; j < map[0].size(); j++)
                {
                    if (std::ranges::contains(route, point(j, i)))
                    {
                        std::cout << 'O';
                    }
                    else
                    {
                        std::cout << map[i][j];
                    }
                }
                std::cout << '\n';
            }
        }
    };

    count = std::numeric_limits<size_t>::max();

    open.push_back({h(start), 0, start, right, {start}});

    auto last_time = std::chrono::high_resolution_clock::now();

    while (!open.empty())
    {

        std::ranges::pop_heap(open, std::greater<>());
        auto [fscore, gscore, p, d, route] = open.back();
        open.pop_back();

        if (gscore > count)
        {
            continue;
        }

        if (p == end)
        {
            if (debug)
            {
                std::println("{} {}", gscore, route.size());
                print_route(route);
            }
            if (gscore < count)
            {
                routes.clear();
                count = gscore;
            }
            if (gscore == count)
            {
                routes.push_back(route);
            }
            continue;
        }

        for (auto d2 : {up, down, left, right})
        {
            if (auto p2 = p + d2; d2 != -d && map[p2.y][p2.x] != '#')
            {
                auto gscore2 = d == d2 ? gscore + 1 : gscore + 1001;
                auto fscore2 = gscore2 + h(p2);
                auto route2 = route;
                route2.push_back(p2);
                if (auto it = std::ranges::find_if(
                        open,
                        [p2, d2, gscore2](std::tuple<
                                          size_t, size_t, point, point,
                                          std::vector<point>>& pair) -> bool
                        {
                            auto& [fscore, gscore, p, d, route] = pair;
                            return p == p2 && d == d2;
                        });
                    it != open.end() && gscore2 < std::get<1>(*it))
                {
                    *it = {fscore2, gscore2, p2, d2, route2};
                    std::ranges::make_heap(open, std::greater<>());
                }
                else if (closed.contains(p2) && std::get<2>(closed[p2]) == d2)
                {
                    if (gscore2 <= std::get<1>(closed[p2]))
                    {
                        closed.erase(p2);
                        open.push_back({fscore2, gscore2, p2, d2, route2});
                        std::ranges::push_heap(open, std::greater<>());
                    }
                }
                else
                {
                    open.push_back({fscore2, gscore2, p2, d2, route2});
                    std::ranges::push_heap(open, std::greater<>());
                }
            }
        }
        closed[p] = {fscore, gscore, d};
        if (debug && std::chrono::high_resolution_clock::now() - last_time >
                         std::chrono::seconds(1))
        {
            last_time = std::chrono::high_resolution_clock::now();
            print_map();
        }
    }

    std::unordered_set<point> spots;
    for (auto& route : routes)
    {
        for (point p : route)
        {
            spots.insert(p);
        }
        if (debug)
        {
            // print_route(route);
        }
    }
    count2 = spots.size();

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}

// 541