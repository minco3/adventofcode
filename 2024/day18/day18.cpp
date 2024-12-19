#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
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

size_t distance(point a, point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    // constexpr size_t width = 7, height = 7, part1_bytes = 12;
    constexpr size_t width = 71, height = 71, part1_bytes = 1024;
    size_t count = 0;
    point last_byte;
    std::string str;
    constexpr bool debug = false;
    std::vector<point> bytes;

    while (std::getline(file, str))
    {
        std::ranges::replace(str, ',', ' ');
        std::stringstream sstr(str);
        int64_t x, y;
        sstr >> x >> y;
        bytes.push_back({x, y});
    }

    std::array<std::array<bool, width>, height> map;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            map[i][j] = false;
        }
    }

    for (auto p : bytes | std::views::take(part1_bytes))
    {
        map[p.y][p.x] = true;
    }

    if (debug)
    {
        for (auto& l : map)
        {
            for (auto b : l)
            {
                std::cout << (b ? '#' : '.');
            }
            std::cout << '\n';
        }
    }

    auto pathfind =
        [width,
         height](std::array<std::array<bool, width>, height>& map) -> size_t
    {
        constexpr point start = {0, 0}, end = {width - 1, height - 1};
        auto h = [end](point p) -> size_t { return distance(p, end); };

        std::vector<std::tuple<size_t, size_t, point>> open = {
            {h(start), 0, start}}; // fscore, gscore
        std::unordered_map<point, size_t> closed;

        if (debug)
        {
            for (auto& l : map)
            {
                for (auto b : l)
                {
                    std::cout << (b ? '#' : '.');
                }
                std::cout << '\n';
            }
        }

        while (!open.empty())
        {
            std::ranges::pop_heap(open, std::greater<>());
            auto [fscore, gscore, p] = open.back();
            open.pop_back();
            if (p == end)
            {
                return gscore;
            }

            for (auto d : {up, down, left, right})
            {
                point p2 = p + d;
                if (contains(p2, width, height) && !map[p2.y][p2.x])
                {
                    if (auto it = std::ranges::find_if(
                            open, [p2](std::tuple<size_t, size_t, point> pair)
                            { return std::get<2>(pair) == p2; });
                        it != open.end())
                    {
                        if (std::get<1>(*it) > gscore + 1)
                        {
                            *it = {gscore + 1 + h(p2), gscore + 1, p2};
                            std::ranges::make_heap(open, std::greater<>());
                        }
                        continue;
                    }
                    else if (closed.contains(p2))
                    {
                        if (closed[p2] > gscore)
                        {
                            closed[p2] = gscore;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    open.push_back({gscore + 1 + h(p2), gscore + 1, p2});
                    std::ranges::push_heap(open, std::greater<>());
                }
            }
            closed[p] = gscore;
        }
        return 0;
    };

    count = pathfind(map);
    size_t cur_step = part1_bytes;
    size_t low = part1_bytes, high = bytes.size();
    while (low < high)
    {
        size_t mid = low + (high - low) / 2;

        // clang-format off
        auto forward = bytes 
                        | std::views::drop(cur_step)
                        | std::views::take(mid - cur_step);
        auto reverse = bytes 
                        | std::views::drop(mid) 
                        | std::views::take(cur_step - mid);
        // clang-format on

        auto view = cur_step < mid ? forward : reverse;

        for (auto byte : view)
        {
            map[byte.y][byte.x] = cur_step < mid ? true : false;
        }

        cur_step = mid;

        if (debug)
        {
            std::cout << low << ' ' << high << ' ' << mid << "\n\n";
        }

        if (pathfind(map))
        {
            last_byte = bytes[mid];
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }

    std::println(
        "{}, {}, {}", count, last_byte,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}