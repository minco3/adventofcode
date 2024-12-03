#include <algorithm>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <unordered_map>

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    bool enabled = true;
    while (std::getline(file, str))
    {
        std::regex re(R"(mul\((\d+),(\d+)\)|do\(\)|don't\(\))");
        for (auto it = std::sregex_iterator(str.begin(), str.end(), re);
             it != std::sregex_iterator(); it++)
        {
            std::smatch match = *it;
            if (match.str() == "do()")
            {
                enabled = true;
            }
            else if (match.str() == "don't()")
            {
                enabled = false;
            }
            else
            {
                count += std::stoi(match[1].str()) * std::stoi(match[2].str());
                if (enabled)
                {
                    count2 +=
                        std::stoi(match[1].str()) * std::stoi(match[2].str());
                }
            }
        }
    }

    std::println("{}, {}", count, count2);
}