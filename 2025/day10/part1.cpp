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
#include <scn/ranges.h>
#include <scn/scan.h>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

template <char Open, char Close>
std::vector<int> parse_brackets(auto& range)
{
    auto open = std::find(range.begin(), range.end(), Open);
    if (open == range.end() || (open + 1) == range.end())
        return {};
    auto close = std::find(open + 1, range.end(), Close);
    if (close == range.end())
        return {};

    std::string content(open + 1, close);
    std::vector<int> result;
    auto cr = scn::ranges::subrange{content};

    while (auto r = scn::scan<int>(cr, "{}"))
    {
        result.push_back(r->value());
        cr = r->range();
        auto comma = std::find(cr.begin(), cr.end(), ',');
        if (comma == cr.end())
            break;
        cr = scn::ranges::subrange{comma + 1, cr.end()};
    }

    range = scn::ranges::subrange{close + 1, range.end()};
    return result;
}

int main()
{
    std::fstream file("input.txt");
    std::string line;
    auto t1 = std::chrono::high_resolution_clock::now();
    int64_t result = 0;

    while (std::getline(file, line))
    {
        uint16_t indicators = 0;
        std::vector<uint16_t> buttons;

        auto r = scn::scan<std::string>(line, "[{:[.#]}]");
        uint64_t N = r->value().length();
        for (auto [i, c] : std::views::enumerate(r->value()))
        {
            if (c == '#')
            {
                indicators |= 1 << i;
            }
        }

        auto remaining = r->range();
        while (true)
        {
            auto t = parse_brackets<'(', ')'>(remaining);
            if (t.empty())
                break;
            uint16_t button = 0;
            for (auto n : t)
            {
                button |= 1 << n;
            }
            buttons.push_back(button);
        }

        // std::println("{:0{}b}, {::0{}b}", indicators, N, buttons, N);

        std::queue<uint16_t> q;
        std::vector<bool> visited(1u << 16, false);
        uint64_t depth = 0;
        q.push(0);
        visited[0] = true;

        while (!q.empty())
        {
            size_t layer_size = q.size();
            for (size_t i = 0; i < layer_size; i++)
            {
                uint16_t cur = q.front();
                q.pop();

                if (cur == indicators)
                {
                    result += depth;
                    q = std::queue<uint16_t>{};
                    // std::println("q: {}, depth: {}", q, depth);
                    break;
                }

                for (auto button : buttons)
                {
                    uint16_t next = cur ^ button;
                    if (!visited[next])
                    {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
            depth++;
        }
    }

    std::unordered_set<uint16_t> visited;

    std::println(
        "result: {}, time: {}", result,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );
}