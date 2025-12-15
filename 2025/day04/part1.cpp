#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::array<std::pair<int64_t, int64_t>, 8> kernel = {
    {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}}};

int main()
{
    std::fstream file("input.txt");
    std::string line;
    uint64_t count = 0;
    std::vector<std::string> grid;
    while (std::getline(file, line))
    {
        grid.push_back(line);
    }
    // std::vector<std::string> grid2 = grid;

    for (int64_t i = 0; i < grid.size(); i++)
    {
        for (int64_t j = 0; j < grid.size(); j++)
        {
            if (grid[i][j] == '.')
            {
                continue;
            }

            uint8_t num = 0;
            for (auto [x, y] : kernel)
            {
                int64_t a = i + x;
                int64_t b = j + y;
                if (a < 0 || a >= grid.size() || b < 0 || b >= grid[0].length())
                {
                    continue;
                }

                if (grid[a][b] == '@')
                {
                    num++;
                }
            }
            if (num < 4)
            {
                count++;
                // grid2[i][j] = 'x';
            }
        }
    }
    // for (auto& line : grid2)
    // {
    //     std::cout << line << '\n';
    // }
    std::cout << count << '\n';
}