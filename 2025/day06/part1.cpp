#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::fstream file("input.txt");
    std::string line;
    std::vector<std::vector<uint64_t>> nums;
    uint64_t total = 0;
    while (std::getline(file, line))
    {
        std::stringstream sstr(line);
        size_t j = 0;
        uint64_t num;
        if (line[0] != '+' && line[0] != '*')
        {
            while (sstr >> num)
            {
                nums.resize(std::max(nums.size(), j + 1));
                nums[j].push_back(num);
                j++;
            }
        }
        else
        {
            std::string str;
            while (sstr >> str)
            {
                auto sum = [](int64_t a, int64_t b) { return a + b; };
                auto product = [](int64_t a, int64_t b) { return a * b; };
                auto op = str == "+" ? sum : product;
                int64_t init = str == "+" ? 0 : 1;
                total += std::ranges::fold_left(nums[j], init, op);
                j++;
            }
        }
    }
    std::cout << total << '\n';
}