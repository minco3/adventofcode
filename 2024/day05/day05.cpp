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
    std::unordered_multimap<int, int> map;
    while (std::getline(file, str))
    {
        std::vector<int> list;
        if (str.size() == 0)
        {
            continue;
        }
        else if (str.find('|') != std::string::npos)
        {
            std::ranges::replace(str, '|', ' ');
            std::stringstream sstr(str);
            int a, b;
            sstr >> a >> b;
            map.insert({a, b});
        }
        else
        {
            std::ranges::replace(str, ',', ' ');
            int a;
            std::stringstream sstr(str);
            while (sstr >> a)
            {
                list.push_back(a);
            }
            for (auto it = list.begin(); it != list.end(); it++)
            {
                if (map.count(*it) &&
                    std::any_of(
                        map.equal_range(*it).first, map.equal_range(*it).second,
                        [&](std::pair<int, int> p)
                        {
                            return std::find(list.begin(), it, p.second) != it;
                        }))
                {
                    break;
                }
                if (it == list.end() - 1)
                {
                    count += list[list.size() / 2];
                }
            }

            // part 2
            bool valid = false, reordered = false;
            while (!valid)
            {
                for (auto it = list.begin(); it != list.end(); it++)
                {
                    auto it2 = std::find_if(
                        list.begin(), it,
                        [&](int n)
                        {
                            return std::any_of(
                                map.equal_range(*it).first,
                                map.equal_range(*it).second,
                                [&](std::pair<int, int> p)
                                { return p.second == n; });
                        });
                    if (it2 != it)
                    {
                        reordered = true;
                        std::swap(*it, *it2);
                        break;
                    }
                    if (it == list.end() - 1)
                    {
                        valid = true;
                    }
                }
            }
            if (reordered)
            {
                count2 += list[list.size() / 2];
            }
        }
    }

    std::println("{}, {}", count, count2);
}