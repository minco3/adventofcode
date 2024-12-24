#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <unordered_map>
#include <unordered_set>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0;
    std::string str;
    constexpr bool debug = false;

    std::unordered_multimap<uint16_t, uint16_t> connections;

    while (std::getline(file, str))
    {
        uint16_t a, b;
        std::string lhs, rhs;
        lhs = str.substr(0, 2);
        rhs = str.substr(3, 2);
        a = (lhs[0] << 8) + lhs[1];
        b = (rhs[0] << 8) + rhs[1];
        connections.insert({a, b});
    }

    std::set<std::array<uint16_t, 3>> triplets;
    for (auto [u, v] : connections) // for all (u,v)
    {
        auto p = connections.equal_range(u);
        auto p2 = connections.equal_range(v);
        for (auto [u, w] :
             std::ranges::subrange(p.first, p.second) |
                 std::ranges::views::filter(
                     [v](auto pair)
                     { return pair.second != v; })) // for all (u,w) where v!=w
        {
            auto p3 = connections.equal_range(w);
            if (std::any_of(
                    p2.first, p2.second,
                    [w](auto pair) { return pair.second == w; }) ||
                std::any_of(
                    p3.first, p3.second,
                    [v](auto pair)
                    {
                        return pair.second == v;
                    })) // check if (v,w) or (w,v) exist
            {
                std::array<uint16_t, 3> vals = {u, v, w};
                std::ranges::sort(vals);
                triplets.insert({vals[0], vals[1], vals[2]});
            }
        }
        for (auto [v, w] :
             std::ranges::subrange(p2.first, p2.second)) // for all (v,w)
        {
            auto p3 = connections.equal_range(w);
            if (std::any_of(
                    p.first, p.second,
                    [w](auto pair) { return pair.second == w; }) ||
                std::any_of(
                    p3.first, p3.second,
                    [u](auto pair)
                    {
                        return pair.second == u;
                    })) // check if (w,u) or (u,w) exist
            {
                std::array<uint16_t, 3> vals = {u, v, w};
                std::ranges::sort(vals);
                triplets.insert({vals[0], vals[1], vals[2]});
            }
        }
    }

    count = std::ranges::count_if(
        triplets,
        [](std::array<uint16_t, 3> arr)
        {
            return std::ranges::any_of(
                arr, [](uint16_t u) { return u >> 8 == 't'; });
        });

    if constexpr (debug)
    {
        std::println("{}", triplets);
    }

    std::function<std::vector<std::vector<uint16_t>>(
        std::vector<uint16_t>, std::vector<uint16_t>, std::vector<uint16_t>,
        std::unordered_map<uint16_t, std::vector<uint16_t>>&)>
        bron_kerbosch =
            [&](std::vector<uint16_t> r, std::vector<uint16_t> p,
                std::vector<uint16_t> x,
                std::unordered_map<uint16_t, std::vector<uint16_t>>& n)
        -> std::vector<std::vector<uint16_t>>
    {
        if (p.empty() && x.empty())
        {
            return {r};
        }
        std::vector<std::vector<uint16_t>> res;
        for (auto it = p.begin(); it != p.end();)
        {
            auto v = *it;
            std::vector<uint16_t>& vn = n[*it];
            std::vector<uint16_t> nr, np, nx;
            nr = r;
            nr.insert(std::ranges::lower_bound(nr, v), v);
            std::ranges::set_intersection(p, vn, std::back_inserter(np));
            std::ranges::set_intersection(x, vn, std::back_inserter(nx));
            auto nres = bron_kerbosch(nr, np, nx, n);
            res.insert_range(res.end(), nres);
            it = p.erase(it);
            x.insert(std::ranges::lower_bound(x, v), v);
        }
        return res;
    };

    std::vector<uint16_t> vertices;
    std::unordered_map<uint16_t, std::vector<uint16_t>> neighbors;

    for (auto [k, v] : connections)
    {
        if (!std::ranges::contains(vertices, k))
            vertices.insert(std::ranges::lower_bound(vertices, k), k);
        if (!std::ranges::contains(vertices, v))
            vertices.insert(std::ranges::lower_bound(vertices, v), v);
        neighbors[k].insert(std::ranges::lower_bound(neighbors[k], v), v);
        neighbors[v].insert(std::ranges::lower_bound(neighbors[v], k), k);
    }

    auto cliques = bron_kerbosch({}, vertices, {}, neighbors);

    auto& largest_clique =
        *std::ranges::max_element(cliques, {}, &std::vector<uint16_t>::size);

    std::string password = "[";
    for (auto vertex : largest_clique)
    {
        password += char(vertex >> 8);
        password += char(vertex & 0xFF);
        password += ',';
    }
    password.back() = ']';

    std::println(
        "{}, {}, {}", count, password,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}