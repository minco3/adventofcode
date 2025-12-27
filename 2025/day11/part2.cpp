#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

int main()
{
    std::fstream file("input.txt");
    std::string line;
    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t result = 0;
    std::vector<std::vector<uint64_t>> graph;
    std::unordered_map<std::string, uint64_t> map;

    while (std::getline(file, line))
    {
        std::string name = line.substr(0, 3);
        uint64_t i;
        if (!map.contains(name))
        {
            map[name] = graph.size();
            graph.push_back({});
        }
        i = map[name];
        std::stringstream sstream(line.substr(5));
        while (std::getline(sstream, name, ' '))
        {
            if (!map.contains(name))
            {
                map[name] = graph.size();
                graph.push_back({});
            }
            graph[i].push_back(map[name]);
        }
    }

    std::vector<int> indeg(graph.size(), 0);
    for (uint64_t i = 0; i < graph.size(); i++)
    {
        for (int j : graph[i])
        {
            indeg[j]++;
        }
    }

    std::vector<uint64_t> topo;
    topo.reserve(graph.size());
    std::queue<uint64_t> q;
    for (uint64_t i = 0; i < graph.size(); i++)
    {
        if (indeg[i] == 0)
        {
            q.push(i);
        }
    }

    while (!q.empty())
    {
        uint64_t i = q.front();
        q.pop();

        topo.push_back(i);

        for (int j : graph[i])
        {
            indeg[j]--;
            if (indeg[j] == 0)
            {
                q.push(j);
            }
        }
    }

    if (topo.size() != graph.size())
    {
        std::println("loop");
        exit(1);
    }

    auto count_paths = [&](uint64_t A, uint64_t B) {
        std::vector<uint64_t> ways(graph.size(), 0);

        ways[A] = 1;

        for (int i : topo)
        {
            for (int j : graph[i])
            {
                ways[j] += ways[i];
            }
        }

        return ways[B];
    };

    result = count_paths(map["svr"], map["dac"])
               * count_paths(map["dac"], map["fft"])
               * count_paths(map["fft"], map["out"])
           + count_paths(map["svr"], map["fft"])
                 * count_paths(map["fft"], map["dac"])
                 * count_paths(map["dac"], map["out"]);

    std::println(
        "result: {}, time: {}", result,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );
}