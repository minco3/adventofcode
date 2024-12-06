#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <set>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

bool contains(point pos, const std::vector<std::string> &map)
{
    return pos.x >= 0 && pos.x < map[0].size() && pos.y >= 0 && pos.y < map.size();
};

size_t solve(point pos, std::vector<std::string> &map)
{
    std::set<point> set;
    point lookdir = {0, -1};
    while (contains(pos, map))
    {
        set.insert(pos);
        if (contains(pos + lookdir, map))
        {
            if (map.at(pos.y + lookdir.y).at(pos.x + lookdir.x) == '#')
            {
                std::swap(lookdir.x, lookdir.y);
                lookdir.x *= -1;
            }
        }
        pos = pos + lookdir;
    }
    return set.size();
}

void loopstep(point &pos, point &lookdir, std::vector<std::string> &map)
{
    while (contains(pos, map))
    {
        if (contains(pos + lookdir, map) && map.at(pos.y + lookdir.y).at(pos.x + lookdir.x) == '#')
        {
            while (map.at(pos.y + lookdir.y).at(pos.x + lookdir.x) == '#')
            {
                std::swap(lookdir.x, lookdir.y);
                lookdir.x *= -1;
            }
            return;
        }
        pos = pos + lookdir;
    }
}

bool has_loop(point pos, std::vector<std::string> &map)
{
    point slow = pos, fast = pos;
    point slowlookdir = {0, -1}, fastlookdir = {0, -1};
    while (fast.x >= 0 && fast.x < map[0].size() && fast.y >= 0 && fast.y < map.size())
    {
        loopstep(slow, slowlookdir, map);
        loopstep(fast, fastlookdir, map);
        loopstep(fast, fastlookdir, map);
        if (slow == fast)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    std::vector<std::string> map;
    point starting_pos;
    while (std::getline(file, str))
    {
        size_t j = str.find('^');
        if (j != std::string::npos)
        {
            starting_pos = {static_cast<int64_t>(j), static_cast<int64_t>(map.size())};
        }
        map.push_back(str);
    }

    count = solve(starting_pos, map);

    for (size_t i = 0; i < map.size(); i++)
    {
        for (size_t j = 0; j < map[0].size(); j++)
        {
            std::vector<std::string> map2 = map;
            if (map2[i][j] == '#' || map2[i][j] == '^')
            {
                continue;
            }
            map2[i][j] = '#';
            if (has_loop(starting_pos, map2))
            {
                count2++;
            }
        }
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}