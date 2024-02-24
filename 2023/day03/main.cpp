#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>

std::array<std::pair<int, int>, 8> kernel = {
    {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}}};

int main()
{
    std::vector<std::string> grid;
    std::vector<std::vector<char>> visited;
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;
    size_t sum = 0;
    while (std::getline(file, str))
    {
        grid.push_back(str);
    }
    std::vector<char> v(str.size(), 0);
    visited.resize(grid.size(), v);
    for (size_t x = 0; x < grid.size(); x++)
    {
        for (size_t y = 0; y < grid.at(x).size(); y++)
        {
            // if (!isdigit(grid[x][y]) && grid[x][y] != '.')
            if (grid[x][y] == '*')
            {
                size_t nnums = 0;
                std::array<size_t, 2> nums;
                for (size_t i = 0; i < kernel.size(); i++)
                {
                    auto p = kernel.at(i);
                    if (x + p.first < 0 || x + p.first >= grid.size() ||
                        y + p.second < 0 || y + p.second >= grid.at(x).size() ||
                        visited[x + p.first][y + p.second])
                    {
                        continue;
                    }
                    if (isdigit(grid[x + p.first][y + p.second]))
                    {
                        if (nnums == 2)
                        {
                            break;
                        }
                        visited[x + p.first][y + p.second] = 1;
                        const std::string& str2 = grid[x + p.first];
                        size_t first = y + p.second, second = y + p.second;

                        while (first > 0 && isdigit(str2.at(first - 1)))
                        {
                            first--;
                            visited[x + p.first][first] = 1;
                        }
                        while (second < str2.size() - 1 &&
                               isdigit(str2.at(second + 1)))
                        {
                            second++;
                            visited[x + p.first][second] = 1;
                        }
                        size_t num;
                        num = std::stoi(&str2.at(first));
                        // sum += num;
                        nums[nnums] = num;
                        nnums++;
                        std::cout << num << std::endl;
                    }
                }
                if (nnums == 2)
                {
                    sum += nums[0] * nums[1];
                }
            }
        }
    }
    std::cout << sum << std::endl;
}