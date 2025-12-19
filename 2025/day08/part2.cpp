#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct point3
{
    uint64_t x, y, z;
};

struct edge
{
    uint64_t distance;
    uint32_t id1, id2;
    auto operator<=>(const edge&) const = default;
};

int main()
{
    std::fstream file("input.txt");
    std::string line;
    std::vector<point3> locations;
    std::vector<edge> edges;
    uint64_t x1, x2;
    auto t1 = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line))
    {
        uint64_t x, y, z;
        std::sscanf(line.c_str(), "%llu,%llu,%llu", &x, &y, &z);
        locations.push_back({x, y, z});
    }

    for (uint32_t i = 0; i < locations.size(); i++)
    {
        for (uint32_t j = i + 1; j < locations.size(); j++)
        {
            point3 box1 = locations[i], box2 = locations[j];
            uint64_t distance_squared = (box1.x - box2.x) * (box1.x - box2.x) +
                                        (box1.y - box2.y) * (box1.y - box2.y) +
                                        (box1.z - box2.z) * (box1.z - box2.z);
            edges.push_back({distance_squared, i, j});
        }
    }

    std::ranges::sort(edges);

    std::vector<uint32_t> comp_id(locations.size());
    std::iota(comp_id.begin(), comp_id.end(), 0);
    std::vector<uint32_t> comp_size(locations.size(), 1);

    for (auto e : edges)
    {
        if (comp_id[e.id1] == comp_id[e.id2])
        {
            continue;
        }
        uint32_t total = comp_size[comp_id[e.id1]] + comp_size[comp_id[e.id2]];
        uint32_t id = std::min(comp_id[e.id1], comp_id[e.id2]);
        if (total == locations.size())
        {
            x1 = locations[e.id1].x;
            x2 = locations[e.id2].x;
            break;
        }
        comp_size[id] = total;
        uint32_t id1 = comp_id[e.id1];
        uint32_t id2 = comp_id[e.id2];
        for (auto& c : comp_id)
        {
            if (c == id1 || c == id2)
            {
                c = id;
            }
        }
    }

    uint64_t result = x1 * x2;

    std::println("{}", comp_size | std::views::take(3));

    std::println(
        "result: {}, time: {}", result,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}