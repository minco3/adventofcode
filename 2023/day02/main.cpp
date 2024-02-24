#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

int main()
{

    std::array<size_t, 3> limit = {12, 13, 14}; // rgb
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;

    size_t total = 0;
    size_t i = 1;

    while (!file.eof())
    {
        std::getline(file, str);

        std::stringstream sstream(str);
        sstream.ignore(INT_MAX, ':');
        std::array<size_t, 3> mins{0};
        while (!sstream.eof())
        {
            std::string str2;
            std::getline(sstream, str2, ';');
            std::stringstream sstream2(str2);
            size_t num;
            std::string color;
            while (!sstream2.eof())
            {
                sstream2 >> num;
                sstream2 >> color;
                switch (color.at(0))
                {
                case 'r':
                    mins[0] = std::max(mins[0], num);
                    break;
                case 'g':
                    mins[1] = std::max(mins[1], num);
                    break;
                case 'b':
                    mins[2] = std::max(mins[2], num);
                    break;
                }
            }
        }
        std::cout << mins[0] << ' ' << mins[1] << ' ' << mins[2] << std::endl;
        total += mins[0] * mins[1] * mins[2];
    }
    std::cout << total;
}