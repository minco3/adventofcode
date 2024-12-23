#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <queue>
#include <ranges>
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

char dtochar(point p)
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
}

// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A |
//     +---+---+
std::vector<std::vector<char>> numpad = {
    {{'7', '8', '9'}, {'4', '5', '6'}, {'1', '2', '3'}, {-1, '0', 'A'}}};
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+
std::vector<std::vector<char>> keypad = {{{-1, '^', 'A'}, {'<', 'v', '>'}}};
point numpad_start = {2, 3}, keypad_start = {2, 0};

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    std::vector<std::string> codes;
    constexpr bool debug = true;

    while (std::getline(file, str))
    {
        codes.push_back(str);
    }

    auto pathfind = [](point start, point end,
                       std::vector<std::vector<char>>& map)
        -> std::vector<std::vector<point>>
    {
        std::vector<
            std::tuple<size_t, size_t, point, point, std::vector<point>>>
            open; // fscore, gscore, pos, d, route
        std::unordered_map<point, std::tuple<size_t, size_t, point>>
            closed; // pos -> fscore, gscore, d
        std::vector<std::vector<point>> routes;

        open.push_back({distance(start, end), 0, start, {0, 0}, {}});
        size_t shortest_path = std::numeric_limits<size_t>::max();

        while (!open.empty())
        {
            std::ranges::pop_heap(open, std::greater<>());
            auto [fscore, gscore, p, d, route] = open.back();
            open.pop_back();
            if (gscore > shortest_path)
            {
                continue;
            }
            if (p == end)
            {
                if (gscore < shortest_path)
                {
                    routes.clear();
                    shortest_path = route.size();
                }
                routes.push_back(route);
                continue;
            }

            for (auto d2 : {up, right, down, left})
            {
                if (auto p2 = p + d2; d2 != -d &&
                                      contains(p2, map[0].size(), map.size()) &&
                                      map[p2.y][p2.x] != -1)
                {
                    auto gscore2 = gscore + 1;
                    auto fscore2 = gscore2 + distance(p2, end);
                    auto route2 = route;
                    route2.push_back(d2);
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
                    else if (closed.contains(p2))
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
        }
        return routes;
    };

    auto find_button = [](char target,
                          std::vector<std::vector<char>>& map) -> point
    {
        for (size_t i = 0; i < map.size(); i++)
        {
            for (size_t j = 0; j < map[0].size(); j++)
            {
                if (map[i][j] == target)
                {
                    return point(j, i);
                }
            }
        }
        return point(-1, -1);
    };

    auto get_instructions =
        [pathfind](
            point start, point end,
            std::vector<std::vector<char>>& map) -> std::vector<std::string>
    {
        static std::unordered_map<std::string, std::vector<std::string>> cache;
        std::string id = std::format(
            "{},{},{}", start, end, reinterpret_cast<intptr_t>(&map));
        if (cache.contains(id))
        {
            return cache[id];
        }
        std::vector<std::string> instructions_list;
        auto routes = pathfind(start, end, map);
        for (auto route : routes)
        {
            std::string instructions;
            for (auto d : route)
            {
                instructions += dtochar(d);
            }
            instructions += "A";
            instructions_list.push_back(instructions);
        }
        cache[id] = instructions_list;
        return instructions_list;
    };

    std::function<size_t(
        point, point, std::vector<std::vector<char>> & map, size_t)>
        get_complexity = [&get_instructions, &find_button, &get_complexity](
                             point start, point end,
                             std::vector<std::vector<char>>& map,
                             size_t depth) -> size_t
    {
        static std::unordered_map<std::string, size_t> cache;
        std::string id = std::format(
            "{},{},{},{}", start, end, reinterpret_cast<intptr_t>(&map), depth);
        if (cache.contains(id))
        {
            return cache[id];
        }
        size_t complexity = std::numeric_limits<size_t>::max();
        std::vector<std::string> instructions_list =
            get_instructions(start, end, map);
        if (depth == 1)
        {
            complexity = instructions_list[0].size();
        }
        else
        {
            for (auto& instructions : instructions_list)
            {
                size_t instructions_complexity = 0;
                point p = keypad_start; // TODO: remove this
                for (auto c : instructions)
                {
                    point p2 = find_button(c, keypad);
                    instructions_complexity +=
                        get_complexity(p, p2, map, depth - 1);
                    p = p2;
                }
                complexity = std::min(complexity, instructions_complexity);
            }
        }
        cache[id] = complexity;
        return complexity;
    };

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

    for (auto& code : codes)
    {
        point p = numpad_start;
        std::vector<std::string> instructions_list = {""};
        for (auto c : code)
        {
            auto p2 = find_button(c, numpad);
            auto instructions_list2 = get_instructions(p, p2, numpad);
            p = p2;
            std::vector<std::string> instructions_list3;
            for (auto& instructions : instructions_list)
            {
                for (auto& instructions2 : instructions_list2)
                {
                    instructions_list3.push_back(instructions + instructions2);
                }
            }
            instructions_list = instructions_list3;
            // if constexpr (debug)
            // {
            //     std::println("{}, {}", c, instructions_list);
            // }
        }
        if constexpr (debug)
        {
            std::println("{}, {}", code, instructions_list);
        }
        size_t complexity = std::numeric_limits<size_t>::max(),
               complexity2 = std::numeric_limits<size_t>::max();
        for (auto& instructions : instructions_list)
        {
            size_t instruction_complexity = 0, instruction2_complexity = 0;
            point p = keypad_start;
            for (char c : instructions)
            {
                point p2 = find_button(c, keypad);
                instruction_complexity += get_complexity(p, p2, keypad, 2);
                instruction2_complexity += get_complexity(p, p2, keypad, 25);
                p = p2;
            }
            complexity = std::min(complexity, instruction_complexity);
            complexity2 = std::min(complexity2, instruction2_complexity);
        }
        if constexpr (debug)
        {
            std::println("{} {} {}", code, complexity, complexity2);
        }
        count += complexity * std::stoull(code.substr(0, 3));
        count2 += complexity2 * std::stoull(code.substr(0, 3));
    }

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}

// <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A
// v<<A>>^A<A>AvA<^AA>A<vAAA>^A
// <v<A>>^A<A>A<AAv>A^A<vAAA^>A
// <A^A>^^AvvvA
// <A^A^^>AvvvA
// 029A

// 126384

// 029A, <v<A>A<A>>^AvAA<^A>A<v<A>>^AvA^A<v<A>>^AA<vA>A^A<A>A<v<A>A^>AAA<Av>A^A

// 029A, <v<A>A<A>>^AvAA<^A>A<v<A>>^AvA^A<v<A>>^AA<vA>A^A<A>A<v<A>A^>AAA<Av>A^A
// 029A: <vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A

// 980A, <v<A>>^AAAvA^A<v<A>A<A>>^AvAA<^A>A<v<A>A^>AAA<Av>A^A<vA^>A<A>A
// 980A: <v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A

// 179A,
// <v<A>A<A>>^AvA<^A>A<vA<A>>^AvAA<^A>A<v<A>>^AAvA^A<vA^>AA<A>A<v<A>A^>AAA<Av>A^A
// 179A: <v<A>>^A<vA<A>>^AAvAA<^A>A<v<A>>^AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A

// 456A, <v<A>>^AA<vA<A>>^AAvAA<^A>A<vA^>A<A>A<vA^>A<A>A<v<A>A^>AA<Av>A^A
// 456A: <v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A

// 379A, <v<A>>^AvA^A<v<A>A<A>>^AAvA<^A>AAvA^A<vA^>AA<A>A<v<A>A^>AAA<Av>A^A
//          <   A > A   < v <   AA >  ^ AA > A  v  AA ^ A   < v  AAA ^  > A
//              ^   A           <<      ^^   A
// 379A: <v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A
//          <   A > A  v <<   AA >  ^ AA > A  v  AA ^ A   < v  AAA >  ^ A
//              ^   A         <<      ^^   A
//                  3                      7
// 379A, <v<A>>^AvA^Av<<A>>^AAv<A<A>>^AAvAA^<A>Av<A>^AA<A>Av<A<A>>^AAAvA^<A>A
//          <   A > A   <   AA  v <   AA >>  ^ A  v  AA ^ A  v <   AAA >  ^ A
//              ^   A       ^^        <<       A     >>   A        vvv      A
//                  3                          7          9                 A

// 624970ms lol