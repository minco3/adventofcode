#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <charconv>
#include <cassert>

int main()
{
    std::fstream file(SOURCE_DIR"/input.txt");
    std::vector<size_t> times, distances, win;
    std::string str, time, distance;
    std::getline(file, str);
    std::stringstream sstr(str);
    sstr.ignore(std::numeric_limits<int>::max(), ':');
    while (!sstr.eof())
    {
        sstr >> str;
        time+=str;
    }
    times.push_back(std::stoull(time));
    std::getline(file, str);
    std::stringstream sstr2(str);
    sstr2.ignore(std::numeric_limits<int>::max(), ':');
    while (!sstr2.eof())
    {
        sstr2 >> str;
        distance += str;

    }
    distances.push_back(std::stoull(distance));
    assert(times.size() == distances.size());
    for (size_t i = 0; i < times.size(); i++)
    {
        win.push_back(0);
        for (size_t t = 0; t < times.at(i); t++)
        {
            if ((times.at(i)-t)*t > distances.at(i))
            {
                win.at(i)++;
            }
        }
    }
    size_t sum = 1;
    for (size_t w : win)
    {
        sum*=w;
    }
    std::cout << sum << std::endl;
}