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

    

    std::println("{}, {}", count, count2);
}