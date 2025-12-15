#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::fstream file("input.txt");
    std::string range;
    uint64_t sum = 0;
    while (std::getline(file, range, ','))
    {
        std::string lower_str = range.substr(0, range.find('-'));
        std::string upper_str = range.substr(range.find('-') + 1);
        uint64_t lower = std::stoull(lower_str);
        uint64_t upper = std::stoull(upper_str);

        if (lower_str.length() % 2 && upper_str.length() % 2)
        {
            continue;
        }
        for (size_t i =
                 std::stoull("0" + lower_str.substr(0, (lower_str.length()) / 2));
             i <=
             std::stoull(upper_str.substr(0, (upper_str.length() + 1) / 2));
             i++)
        {
            uint64_t num =
                i * pow(10, 1 + static_cast<uint64_t>(floor(log10(i)))) + i;
            if (lower <= num && num <= upper)
            {
                sum += num;
            }
        }
    }
    std::cout << sum << '\n';
}