#include <algorithm>
#include <fstream>
#include <functional>
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
            std::string op;
            while (sstr >> op)
            {
                if (op == "+")
                {
                    total += std::ranges::fold_left(
                        nums[j], 0LL, std::plus<uint64_t>{});
                }
                else if (op == "*")
                {
                    total += std::ranges::fold_left(
                        nums[j], 1LL, std::multiplies<uint64_t>{});
                }
                j++;
            }
        }
    }
    std::cout << total << '\n';
}