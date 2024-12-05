#include <algorithm>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <unordered_map>

constexpr const std::string XMAS = "XMAS";

bool bfs(std::vector<std::string>& map, ptrdiff_t i, ptrdiff_t j, size_t c)
{
    if (c == XMAS.size())
    {
        return true;
    }

    return true;
}

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    std::vector<std::string> map;
    while (std::getline(file, str))
    {
        map.push_back(str);
    }

    for (ptrdiff_t i = 0; i < map.size(); i++)
    {
        for (ptrdiff_t j = 0; j < map[0].size(); j++)
        {
            if (map[i][j] == 'X')
            {
                for (ptrdiff_t y = -1; y <= 1; y++)
                {
                    for (ptrdiff_t x = -1; x <= 1; x++)
                    {
                        for (size_t c = 0; c <= XMAS.size(); c++)
                        {
                            if (c == XMAS.size())
                            {
                                count++;
                                break;
                            }
                            if (i + c * y >= 0 && i + c * y < map.size() &&
                                j + c * x >= 0 && j + c * x < map[0].size() &&
                                map[i + c * y][j + c * x] == XMAS[c])
                            {
                                continue;
                            }
                            break;
                        }
                    }
                }
            }
            if (i > 0 && i < map.size() - 1 && j > 0 && j < map[0].size() - 1 &&
                map[i][j] == 'A')
            {
                if (((map[i - 1][j - 1] == 'S' && map[i + 1][j + 1] == 'M') ||
                     (map[i - 1][j - 1] == 'M' && map[i + 1][j + 1] == 'S')) &&
                    ((map[i - 1][j + 1] == 'S' && map[i + 1][j - 1] == 'M') ||
                     (map[i - 1][j + 1] == 'M' && map[i + 1][j - 1] == 'S')))

                {
                    count2++;
                }
            }
        }
    }

    std::println("{}, {}", count, count2);
}