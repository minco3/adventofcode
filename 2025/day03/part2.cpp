#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

constexpr const uint64_t NUM_ELEMS = 12;

int main()
{
    std::string joltage;
    uint64_t sum = 0;
    std::fstream file("input.txt");
    while (std::getline(file, joltage))
    {
        uint64_t num = 0;
        auto it = joltage.begin();

        for (uint64_t i = 0; i < NUM_ELEMS; i++)
        {
            it = std::ranges::max_element(
                std::ranges::subrange(
                    it, std::prev(joltage.end(), NUM_ELEMS - 1 - i)));
            num = num * 10 + *it - '0';
            it++;
        }
        std::cout << num << '\n';
        sum += num;
    }
    std::cout << sum << '\n';
}