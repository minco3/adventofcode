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
    uint64_t count = 0;
    std::map<uint64_t, bool> ranges; // TRUE = is_fresh
    ranges.insert({std::numeric_limits<uint64_t>::max(), false});
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            break;
        }

        uint64_t lower = std::stoull(line.substr(0, line.find('-')));
        uint64_t upper = std::stoull(line.substr(line.find('-') + 1));

        if (auto it = ranges.lower_bound(lower);
            it == ranges.begin() ||
            (std::prev(it)->second && std::prev(it)->first < lower - 1))
        {
            ranges[lower - 1] = false;
        }
        if (!ranges.upper_bound(upper)->second)
        {
            ranges[upper] = true;
        }

        auto lower_it = ranges.lower_bound(lower);
        auto upper_it = ranges.lower_bound(upper);
        if (lower_it->first <= upper)
        {
            if (std::prev(lower_it)->second)
            {
                lower_it--;
            }
            if (std::next(upper_it)->second)
            {
                upper_it++;
            }
            ranges.erase(lower_it, upper_it);
        }
    }

    for (auto it = ranges.begin(); it != std::prev(ranges.end()); std::advance(it, 2))
    {
        count += std::next(it)->first - it->first;
    }
    std::cout << count << '\n';
}