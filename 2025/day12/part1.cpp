#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::fstream file("input.txt");
    std::string line;
    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<int> shapeCells;
    std::vector<std::string> currentGrid;
    bool readingShape = false;

    std::vector<std::tuple<int, int, std::vector<int>>> problems;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            if (!currentGrid.empty())
            {
                int cells = 0;
                for (const auto& row : currentGrid)
                {
                    cells += std::ranges::count(row, '#');
                }
                shapeCells.push_back(cells);
                currentGrid.clear();
            }
            readingShape = false;
            continue;
        }

        if (line.back() == ':' && line.find('x') == std::string::npos)
        {
            readingShape = true;
            continue;
        }

        auto xPos = line.find('x');
        auto colonPos = line.find(':');
        if (xPos !=` std::string::npos && colonPos != std::string::npos && xPos < colonPos)
        {
            int width = std::stoi(line.substr(0, xPos));
            int height = std::stoi(line.substr(xPos + 1, colonPos - xPos - 1));

            std::vector<int> counts;
            std::istringstream iss(line.substr(colonPos + 1));
            int count;
            while (iss >> count)
            {
                counts.push_back(count);
            }

            problems.emplace_back(width, height, counts);
            continue;
        }

        if (readingShape)
        {
            currentGrid.push_back(line);
        }
    }

    if (!currentGrid.empty())
    {
        int cells = 0;
        for (const auto& row : currentGrid)
        {
            cells += std::ranges::count(row, '#');
        }
        shapeCells.push_back(cells);
    }

    int possible = 0;

    for (const auto& [width, height, counts] : problems)
    {
        int64_t gridArea = static_cast<int64_t>(width) * height;
        int64_t totalShapeCells = 0;

        for (size_t i = 0; i < counts.size() && i < shapeCells.size(); ++i)
        {
            totalShapeCells += static_cast<int64_t>(counts[i]) * shapeCells[i];
        }

        if (totalShapeCells <= gridArea)
        {
            ++possible;
        }
    }

    std::println(
        "result: {}, time: {}",
        possible,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1
        )
    );

    return 0;
}
