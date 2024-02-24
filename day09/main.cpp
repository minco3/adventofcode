#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include <functional>

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    if (!file.is_open())
    {
        std::cout << "file didn't open" << std::endl;
        exit(1);
    }
    std::vector<int64_t> mem;
    while (!file.eof())
    {
        std::string str;
        std::getline(file, str, ',');
        mem.push_back(stoll(str));
    }
    mem.resize(UINT16_MAX, 0);
    int64_t relbase = 0;
    for (size_t i = 0; i < mem.size();)
    {
        int64_t op = mem.at(i) % 100, p1 = (mem.at(i) % 1000) / 100, p2 = (mem.at(i) % 10000) / 1000, p3 = (mem.at(i) % 100000) / 10000;
        try
        {
            auto resolvemode = [&](uint64_t mode, int64_t i)
            {
                switch (mode)
                {
                case 0:
                    return mem.at(i);
                case 1:
                    return i;
                case 2:
                    return relbase + mem.at(i);
                default:
                {
                    std::cout << "unknown mode\n";
                    exit(1);
                }
                }
            };
            p1 = resolvemode(p1, i+1);
            p2 = resolvemode(p2, i+2);
            p3 = resolvemode(p3, i+3);
        }
        catch (...)
        {
            // yummy exceptions
        }

        switch (op)
        {
        case 1: // ADD
            mem.at(p3) = mem.at(p1) + mem.at(p2);
            i += 4;
            break;
        case 2: // MUL
            mem.at(p3) = mem.at(p1) * mem.at(p2);
            i += 4;
            break;
        case 3: // IN
            std::cout << "input requested\n";
            std::cin >> op;
            mem.at(p1) = op;
            i += 2;
            break;
        case 4: // OUT
            std::cout << "output: " << mem.at(p1) << '\n';
            i += 2;
            break;
        case 5: // JE
            i = mem.at(p1) ? mem.at(p2) : i + 3;
            break;
        case 6: // JNE
            i = !mem.at(p1) ? mem.at(p2) : i + 3;
            break;
        case 7: // LE
            mem.at(p3) = mem.at(p1) < mem.at(p2) ? 1 : 0;
            i += 4;
            break;
        case 8: // EQ
            mem.at(p3) = mem.at(p1) == mem.at(p2) ? 1 : 0;
            i += 4;
            break;
        case 9: // adjustrelbase
            relbase += mem.at(p1);
            i += 2;
            break;
        case 99: // EXIT
            std::cout << "exited successfully" << std::endl;
            exit(0);
        default:
            std::cout << "unknown opcode" << std::endl;
            exit(1);
        }
    }
}