#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>
#include <tuple>
#include <vector>
#include <numeric>

int main()
{
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;
    size_t sum = 0;
    bool winning = true;
    std::vector<size_t> cards;
    size_t i = 0;
    while (std::getline(file, str))
    {
        std::vector<size_t> win;
        std::vector<size_t> my;
        std::stringstream sstr(str);
        sstr.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        std::getline(sstr, str, '|');
        std::stringstream sstr2(str);
        while (!sstr2.eof())
        {
            size_t num;
            sstr2 >> num;
            win.push_back(num);
        }
        sstr.ignore(1);
        std::getline(sstr, str);
        std::stringstream sstr3(str);
        while (!sstr3.eof())
        {
            size_t num;
            sstr3 >> num;
            my.push_back(num);
        }
        size_t value = 0;
        for (size_t m : my)
        {
            if (std::find(win.begin(), win.end(), m) != win.end())
            {
                value++;
            }
        }
        if (i+value >= cards.size())
        {
            cards.resize(i+value+1, 1);
        }
        for (size_t j = 0; j < value; j++)
        {
           cards[i+j+1]+=cards[i]; 
        }
        if (value)
        {
            sum += pow(2, value - 1);
        }
        i++;
    }
    size_t sum2 = std::accumulate(cards.begin(), cards.end(), 0);
    std::cout << sum << ' ' << sum2 << std::endl;
}