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

constexpr const size_t NUM_CONNECTIONS = 1000;

int main()
{
    std::fstream file("input.txt");
    std::string line;
    std::vector<point3> locations;
    std::priority_queue<edge> edges;
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

            if (edges.size() < NUM_CONNECTIONS)
            {
                edges.push({distance_squared, i, j});
            }
            else if (edges.top().distance > distance_squared)
            {
                edges.pop();
                edges.push({distance_squared, i, j});
            }
        }
    }
    std::vector<std::vector<uint32_t>> adj(locations.size());

    for (auto [distance, u, v] : edges.__get_container())
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<uint32_t> comp_id(
        locations.size(), std::numeric_limits<uint32_t>::max());
    std::vector<uint32_t> comp_size;
    uint32_t cid = 0;

    for (uint32_t i = 0; i < locations.size(); i++)
    {
        std::queue<uint32_t> q;

        if (comp_id[i] != std::numeric_limits<uint32_t>::max())
        {
            continue;
        }

        q.push(i);
        uint32_t size = 0;
        comp_id[i] = cid;

        while (!q.empty())
        {
            uint32_t j = q.front();
            q.pop();
            size++;
            for (uint32_t k : adj[j])
            {
                if (comp_id[k] == std::numeric_limits<uint32_t>::max())
                {
                    q.push(k);
                    comp_id[k] = cid;
                }
            }
        }

        comp_size.push_back(size);
        cid++;
    }

    std::ranges::sort(comp_size, std::greater<uint32_t>{});

    uint64_t result = std::ranges::fold_left(
        comp_size | std::views::take(3), 1, std::multiplies<uint64_t>{});

    std::println("{}", comp_size | std::views::take(3));

    std::println(
        "result: {}, time: {}", result,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}