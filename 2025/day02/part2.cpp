#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

int main()
{
    std::string range;
    uint64_t sum = 0;
    std::fstream file("input.txt");
    while (std::getline(file, range, ','))
    {
        std::unordered_set<uint64_t> invalid_ids;
        std::string lower_str = range.substr(0, range.find('-'));
        std::string upper_str = range.substr(range.find('-') + 1);
        uint64_t lower = std::stoull(lower_str);
        uint64_t upper = std::stoull(upper_str);
        
        for (size_t i = 2; i <= std::max(lower_str.length(), upper_str.length());
        i++)
        {
            if (lower_str.length() % i && upper_str.length() % i)
            {
                continue;
            }
            for (size_t j = std::stoull(
                     "0" + lower_str.substr(0, (lower_str.length()) / i));
                 j <=
                 std::stoull(upper_str.substr(0, (upper_str.length() + (i-1)) / i));
                 j++)
            {
                uint64_t num = j;
                for (size_t k = 0; k < i - 1; k++)
                {
                    num *= pow(10, 1 + static_cast<uint64_t>(floor(log10(j))));
                    num += j;
                }
                if (lower <= num && num <= upper && !invalid_ids.count(num))
                {
                    invalid_ids.insert(num);
                    sum += num;
                }
            }
        }
    }
    std::cout << sum << '\n';
}