#include "mdspan.hpp"
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

uint64_t solve(std::vector<uint16_t> buttons, std::vector<uint64_t> joltages)
{
    uint64_t N = joltages.size() + 1;
    uint64_t M = buttons.size() + joltages.size() + 1;
    double penalty = 10000;
    std::vector<double> m(N * M, 0.0);
    Kokkos::mdspan<double, Kokkos::dextents<size_t, 2>, Kokkos::layout_left> matrix(
        m.data(), M, N
    );

    for (uint64_t i = 0; i < joltages.size(); i++)
    {
        matrix.at(M - 1, i) = double(joltages[i]);
    }
    for (uint64_t i = 0; i < buttons.size(); i++)
    {
        uint64_t button_count = 0;
        for (uint64_t j = 0; j < joltages.size(); j++)
        {
            if (buttons[i] & 1 << j)
            {
                matrix.at(i, j) = 1.0;
                button_count++;
            }
        }
        matrix.at(i, N - 1) = button_count * penalty - 1.0;
    }
    for (uint64_t i = 0; i < joltages.size(); i++)
    {
        matrix.at(buttons.size() + i, i) = 1.0;
    }

    matrix.at(M - 1, N - 1) = std::ranges::fold_left(
                                  joltages, 0, std::plus<uint64_t>()
                              )
                            * penalty;

    // for (size_t j = 0; j < matrix.extent(1); j++)
    // {
    //     for (size_t i = 0; i < matrix.extent(0); i++)
    //     {
    //         std::print("[{:8}]", matrix.at(i, j));
    //     }
    //     std::cout << '\n';
    // }
    // std::cout << '\n';

    while (true)
    {
        uint64_t pivot_col = -1;
        double max_val = 1e-9;

        for (uint64_t j = 0; j < M - 1; j++)
        {
            if (matrix.at(j, N - 1) > max_val)
            {
                max_val = matrix.at(j, N - 1);
                pivot_col = j;
            }
        }
        if (pivot_col == -1)
        {
            break;
        }

        uint64_t pivot_row = -1;
        double min_ratio = std::numeric_limits<double>::infinity();

        for (uint64_t i = 0; i < N - 1; ++i)
        {
            double coef = matrix.at(pivot_col, i);

            if (coef > 1e-9)
            {
                double rhs = matrix.at(M - 1, i);
                double ratio = rhs / coef;

                if (ratio < min_ratio)
                {
                    min_ratio = ratio;
                    pivot_row = i;
                }
            }
        }

        double pivot_val = matrix.at(pivot_col, pivot_row);

        for (uint64_t j = 0; j < M; ++j)
        {
            matrix.at(j, pivot_row) /= pivot_val;
        }

        for (uint64_t i = 0; i < N; ++i)
        {
            if (i != pivot_row)
            {
                double factor = matrix.at(pivot_col, i);
                if (std::abs(factor) > 1e-9)
                {
                    for (uint64_t j = 0; j < M; ++j)
                    {
                        matrix.at(j, i) -= factor * matrix.at(j, pivot_row);
                    }
                }
            }
        }

        // for (size_t j = 0; j < matrix.extent(1); j++)
        // {
        //     for (size_t i = 0; i < matrix.extent(0); i++)
        //     {
        //         std::print("[{:8.2}]", matrix.at(i, j));
        //     }
        //     std::cout << '\n';
        // }
        // std::cout << '\n';
    }

    return std::ceil(std::abs(matrix.at(M - 1, N - 1)));
}

template <char Open, char Close>
std::vector<uint64_t> parse_brackets(auto& range)
{
    auto open = std::find(range.begin(), range.end(), Open);
    if (open == range.end() || (open + 1) == range.end())
    {
        return {};
    }
    auto close = std::find(open + 1, range.end(), Close);
    if (close == range.end())
    {
        return {};
    }

    std::string content(open + 1, close);
    std::vector<uint64_t> result;
    auto cr = scn::ranges::subrange{content};

    while (auto r = scn::scan<int>(cr, "{}"))
    {
        result.push_back(r->value());
        cr = r->range();
        auto comma = std::find(cr.begin(), cr.end(), ',');
        if (comma == cr.end())
        {
            break;
        }
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

        auto joltages = parse_brackets<'{', '}'>(remaining);
        // std::println("{:0{}b}, {::0{}b}, {}", indicators, N, buttons, N, joltages);

        result += solve(buttons, joltages);
    }

    std::println(
        "result: {}, time: {}", result,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );
}