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
    uint64_t sum=0;
    while (!file.eof())
    {
        std::string str;
        std::getline(file, str);
        int32_t num = stoi(str)/3-2;
        while (num > 0)
        {
            sum += num;
            num = num/3-2;
        }
    }
    std::cout << sum << std::endl;
}