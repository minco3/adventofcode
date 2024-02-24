#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

struct mapping_t
{
    size_t in, out, range;
};

int main()
{

    std::vector<std::pair<size_t, size_t>> ranges;
    std::vector<std::vector<mapping_t>> maps;
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;

    // get seeds
    std::getline(file, str);
    std::stringstream sstr(str);
    size_t sstart, length;
    sstr.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    while (!sstr.eof())
    {
        sstr >> sstart >> length;
        ranges.emplace_back(sstart, length);
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<mapping_t> m;
    while (std::getline(file, str))
    {
        if (str.empty())
        {
            maps.push_back(m);
            m.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::stringstream sstream(str);
        mapping_t ma;
        sstream >> ma.out >> ma.in >> ma.range;
        m.push_back(ma);
    }
    maps.push_back(m);

    for (const auto& map : maps)
    {
        std::vector<std::pair<size_t, size_t>> new_ranges;
        for (auto& range : ranges)
        {
            // split
            auto& [start, length] = range;
            size_t end = start + length - 1;
            for (auto [in, out, range] : map)
            {
                if (start >= in && start + length <= in + range)
                {
                    // do nothing
                    break;
                }
                else if (start >= in && start < in + range)
                {
                    new_ranges.emplace_back(
                        in + range,
                        start + length - in - range); // after end
                    length = in + range - start;
                    break;
                }
                else if (start + length <= in + range && start + length > in)
                {
                    new_ranges.emplace_back(start, in - start); // before start
                    length = length - in + start;
                    start = in;
                    break;
                }
                else if (start < in && start + length > in + range)
                {
                    // split twice
                    new_ranges.emplace_back(start, in - start); // before start
                    new_ranges.emplace_back(
                        in + range,
                        start + length - in - range); // after end
                    start = in;
                    length = in + range - start;
                    break;
                }
            }
        }
        ranges.insert(ranges.end(), new_ranges.begin(), new_ranges.end());
        for (auto& [start, length] : ranges)
        {
            // map
            for (auto [in, out, range] : map)
            {
                if (start >= in && start < in + range)
                {
                    start = start - in + out;
                    break;
                }
            }
        }
    }

    size_t min = std::ranges::min(ranges | std::views::keys);
    std::cout << min;
}