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
    std::vector<uint32_t> vec;
    while (!file.eof())
    {
        std::string str;
        std::getline(file, str, ',');
        vec.push_back(stoi(str));
    }
    for (size_t x = 0; x < 99; x++)
    {
        for (size_t y = 0; y < 99; y++)
        {
            std::vector<uint32_t> mem(vec);
            mem[1] = x;
            mem[2] = y;
            try
            {
                for (size_t i = 0; i < mem.size(); i += 4)
                {

                    switch (vec[i])
                    {
                    case 1:
                        mem.at(mem.at(i + 3)) =
                            mem.at(mem.at(i + 1)) + mem.at(mem.at(i + 2));
                            break;
                    case 2:
                        mem.at(mem.at(i + 3)) =
                            mem.at(mem.at(i + 1)) * mem.at(mem.at(i + 2));
                            break;
                    case 99:
                        if (mem[0] == 19690720)
                        {
                            std::cout << x * 100 + y << std::endl;
                        }
                        std::cout << mem[0] << std::endl;
                        goto end;
                    }
                }
                end:
            }
            catch (...)
            {
                // std::cout << "segfault" << std::endl;
            }
        }
    }
}