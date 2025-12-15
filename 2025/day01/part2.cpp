#include <fstream>
#include <iostream>
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
        
        count += number / NUM_POS;
        number = number % NUM_POS;
        
        if (line[0] == 'R') // CW
        {
            int32_t old_pos = pos;
            pos = (pos + number) % NUM_POS;
            if (pos < old_pos)
            {
                count++;
            }
        }
        else // CCW
        {
            int32_t old_pos = pos;
            pos = (((pos - number) % NUM_POS) + NUM_POS) % NUM_POS;
            if (pos > old_pos && old_pos != 0)
            {
                count++;
            }
            if (pos == 0 && old_pos != 0)
            {
                count++;
            }
        }
    }

    std::cout << count << '\n';
}