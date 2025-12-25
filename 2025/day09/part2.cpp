#include "mdspan.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct point2
{
    int64_t x, y;
};

int main()
{
    std::fstream file("input.txt");
    std::string line;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::vector<point2> points;
    int64_t max = 0;
    while (std::getline(file, line))
    {
        int64_t x, y;
        std::sscanf(line.c_str(), "%llu,%llu", &x, &y);
        points.push_back({x, y});
    }
    points.push_back(points.front()); // close the loop

    auto x_unique = points
                  | std::views::transform(&point2::x)
                  | std::ranges::to<std::vector>();

    std::ranges::sort(x_unique);
    auto [xbegin, xend] = std::ranges::unique(x_unique);
    x_unique.erase(xbegin, xend);

    auto y_unique = points
                  | std::views::transform(&point2::y)
                  | std::ranges::to<std::vector>();

    std::ranges::sort(y_unique);
    auto [ybegin, yend] = std::ranges::unique(y_unique);
    y_unique.erase(ybegin, yend);

    std::vector<int64_t> prefix_sum(x_unique.size() * y_unique.size(), 0);
    auto prefix_span = Kokkos::mdspan<int64_t, Kokkos::dextents<size_t, 2>, Kokkos::layout_left>(
        prefix_sum.data(), Kokkos::extents{x_unique.size(), y_unique.size()}
    );

    std::vector<double> row_intersections;
    row_intersections.reserve(points.size());

    for (size_t j = 0; j < y_unique.size() - 1; ++j)
    {
        double y_mid = (static_cast<double>(y_unique[j]) + static_cast<double>(y_unique[j + 1])) / 2.0;
        int64_t row_height = y_unique[j + 1] - y_unique[j];

        row_intersections.clear();
        for (auto [p1, p2] : points | std::views::adjacent<2>)
        {
            double min_y = static_cast<double>(std::min(p1.y, p2.y));
            double max_y = static_cast<double>(std::max(p1.y, p2.y));

            if (y_mid > min_y && y_mid < max_y)
            {
                double dx = static_cast<double>(p2.x - p1.x);
                double dy = static_cast<double>(p2.y - p1.y);
                double x_int = static_cast<double>(p1.x) + (y_mid - static_cast<double>(p1.y)) * (dx / dy);
                row_intersections.push_back(x_int);
            }
        }

        std::ranges::sort(row_intersections);

        bool inside = false;
        size_t k = 0;

        for (size_t i = 0; i < x_unique.size() - 1; ++i)
        {
            double x_mid_real = (static_cast<double>(x_unique[i]) + static_cast<double>(x_unique[i + 1])) / 2.0;
            int64_t col_width = x_unique[i + 1] - x_unique[i];

            while (k < row_intersections.size() && row_intersections[k] < x_mid_real)
            {
                inside = !inside;
                k++;
            }

            uint64_t cell_area = inside ? (col_width * row_height) : 0;

            prefix_span.at(i + 1, j + 1) = cell_area
                                         + prefix_span.at(i + 1, j)
                                         + prefix_span.at(i, j + 1)
                                         - prefix_span.at(i, j);
        }
    }

    // for (auto j : std::views::iota(0ull, y_unique.size()))
    // {
    //     for (auto i : std::views::iota(0ull, x_unique.size()))
    //     {
    //         std::print("{:2},", prefix_span.at(i, j));
    //     }
    //     std::cout << '\n';
    // }

    std::vector<point2> indices = points
                                | std::views::transform([&](auto p) {
                                      return point2{
                                          std::distance(x_unique.begin(), std::ranges::lower_bound(x_unique, p.x)),
                                          std::distance(y_unique.begin(), std::ranges::lower_bound(y_unique, p.y))
                                      };
                                  })
                                | std::ranges::to<std::vector>();

    for (uint32_t i = 0; i < points.size(); i++)
    {
        for (uint32_t j = i + 1; j < points.size(); j++)
        {
            auto [min_x, max_x] = std::minmax(points[i].x, points[j].x);
            auto [min_y, max_y] = std::minmax(points[i].y, points[j].y);

            auto [x1, x2] = std::minmax(indices[i].x, indices[j].x);
            auto [y1, y2] = std::minmax(indices[i].y, indices[j].y);

            uint64_t estimated_area = (max_x - min_x) * (max_y - min_y);

            uint64_t actual_area = prefix_span.at(x2, y2)
                                 - prefix_span.at(x1, y2)
                                 - prefix_span.at(x2, y1)
                                 + prefix_span.at(x1, y1);

            if (estimated_area == actual_area)
            {
                max = std::max(max, (max_x - min_x + 1) * (max_y - min_y + 1));
            }

            // std::println("min_x: {}, max_x: {}, min_y: {}, max_y: {}, est: {}, act: {}", min_x, max_x, min_y, max_y, estimated_area, actual_area);
        }
    }

    std::println(
        "result: {}, time: {}", max,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );
}