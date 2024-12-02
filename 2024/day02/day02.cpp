#include <algorithm>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <unordered_map>

bool check(std::vector<int>& vec)
{
    int sign = 0;
    auto r = std::ranges::subrange(vec.begin()+1, vec.end());
    int n = vec[0];
    for (auto n2 : r)
    {
        if (!sign)
        {
            sign = n2 - n < 0 ? -1 : 1;
        }

        if (sign != (n2 - n < 0 ? -1 : 1) || std::abs(n2 - n) > 3 ||
            std::abs(n2 - n) == 0)
        {
            return false;
        }
        n = n2;

    }
    return true;
}

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    std::vector<int> l, r;
    size_t count = 0, count2 = 0;
    while (std::getline(file, str))
    {
        std::stringstream sstr(str);
        auto vec = std::vector<int>(std::istream_iterator<int>(sstr), std::istream_iterator<int>());
        if (check(vec))
        {
            count++;
            count2++;
        }
        else
        {
            for (size_t i = 0; i < vec.size(); i++)
            {
                std::vector<int> vec2 = vec;
                vec2.erase(vec2.begin() + i);
                std::println("{}, {}", vec2, i+1);
                if (check(vec2))
                {
                    count2++;
                    break;
                }
            }
        }
    }

    std::println("{}, {}", count, count2);
}