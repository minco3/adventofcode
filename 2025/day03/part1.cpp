#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

int main()
{
    std::string joltage;
    uint64_t sum = 0;
    std::fstream file("sample1.txt");
    while (std::getline(file, joltage))
    {
        uint64_t max = 0;
        for (uint64_t i = 0; i < joltage.size(); i++)
        {
            for (uint64_t j = i + 1; j < joltage.size(); j++)
            {
                max = std::max(
                    max, static_cast<uint64_t>(
                             (joltage[i] - '0') * 10 + joltage[j] - '0'));
            }
        }
        std::cout << max << '\n';
        sum += max;
    }
    std::cout << sum << '\n';
}