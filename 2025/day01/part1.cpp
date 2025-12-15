#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

constexpr const int32_t NUM_POS = 100;

int main()
{
    std::fstream file("input.txt");
    std::string line;
    int32_t pos = 50, count = 0;
    while (std::getline(file, line))
    {
        int32_t number = std::stoul(line.substr(1));
        if (line[0] == 'R')
        {
            pos = (pos + number) % NUM_POS;
        }
        else
        {
            pos = (((pos - number) % NUM_POS) + NUM_POS) % NUM_POS;
        }
        if (pos == 0)
        {
            count ++;
        }
    }

    std::cout << count << '\n';
}