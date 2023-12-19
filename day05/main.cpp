#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>

struct mapping_t
{
    size_t in, out, range;
};

int main()
{

    std::vector<std::pair<size_t, size_t>> seeds;
    std::vector<std::vector<mapping_t>> maps;
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;

    // get seeds
    std::getline(file, str);
    std::stringstream sstr(str);
    size_t sstart, length;
    sstr.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    while (!sstr.eof())
    {
        sstr >> sstart >> length;
        seeds.push_back({sstart, length});
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

    std::vector<std::pair<size_t, size_t>> from = seeds, to;
    for (size_t i = 0; i < maps.size(); i++)
    {
        for (size_t x = 0; x < from.size(); x++)
        {
            // split
            size_t start = from[x].first,
                   end = from[x].first + from[x].second - 1;
            for (size_t j = 0; j < maps[i].size(); j++)
            {
                size_t mstart = maps[i][j].in,
                       mend = maps[i][j].in + maps[i][j].range - 1;
                if (start >= mstart && start <= mend && end <= mend &&
                    end >= mstart)
                {
                    // do nothing
                    break;
                }
                else if (start >= mstart && start <= mend)
                {
                    from.push_back({mend + 1, end - mend}); // after end
                    end = mend;
                }
                else if (end <= mend && end >= mstart)
                {
                    from.push_back({start, mstart - start}); // before start
                    start = mstart;
                }
                else if (start < mstart && end > mend)
                {
                    // split twice
                    from.push_back({start, mstart - start}); // before start
                    from.push_back({mend+1, end - mend});      // after end
                    start = mstart;
                    end = mend;
                }
            }
            from[x] = {start, end - start + 1};
        }
        for (size_t x = 0; x < from.size(); x++)
        {
            // map
            for (size_t j = 0; j < maps[i].size(); j++)
            {
                if (from[x].first >= maps[i][j].in &&
                    from[x].first < maps[i][j].in + maps[i][j].range)
                {
                    to.push_back(
                        {from[x].first - maps[i][j].in + maps[i][j].out,
                         from[x].second});
                         break;
                }
                if (j == maps[i].size()-1)
                {
                    to.push_back(from[x]);
                }
            }
        }
        from = to;
        to.clear();
    }

    size_t min = std::ranges::min(from | std::views::keys);
    std::cout << min;
}