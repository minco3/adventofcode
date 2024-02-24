#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    if (!file.is_open())
    {
        std::cout << "file didn't open" << std::endl;
        exit(1);
    }
    std::vector<uint32_t> mem;
    while (!file.eof())
    {
        std::string str;
        std::getline(file, str, ',');
        mem.push_back(stoi(str));
    }
    for (size_t i = 0; i < mem.size();)
    {

        switch (mem[i])
        {
        case 1:
            mem.at(mem.at(i + 3)) =
                mem.at(mem.at(i + 1)) + mem.at(mem.at(i + 2));
            i += 4;
            break;
        case 2:
            mem.at(mem.at(i + 3)) =
                mem.at(mem.at(i + 1)) * mem.at(mem.at(i + 2));
            i += 4;
            break;
        case 3:

            i += 2;
            break;
        case 4:
            i += 2;
            break;
        case 99:
            std::cout << mem[0] << std::endl;
            exit(0);
        }
    }
}