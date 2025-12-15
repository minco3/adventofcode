#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::fstream file("input.txt");
    std::string line;
    std::vector<std::string> lines;
    uint64_t total = 0;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    auto sum = [](uint64_t a, uint64_t b) { return a + b; };
    auto product = [](uint64_t a, uint64_t b) { return a * b; };
    std::function<uint64_t(uint64_t, uint64_t)> op;
    uint64_t num;
    for (size_t i = 0; i < lines[0].length(); i++)
    {
        for (size_t j = 0; j < lines.size(); j++)
        {
            if (lines[j][i] != ' ')
            {
                break;
            }
            if (j == lines.size() - 1)
            {
                total += num;
            }
        }
        std::string str;
        for (size_t j = 0; j < lines.size(); j++)
        {
            if (isnumber(lines[j][i]))
            {
                str.push_back(lines[j][i]);
            }
        }
        uint64_t number = str.empty() ? 0 : std::stoull(str);
        if (lines.back()[i] == '+')
        {
            op = sum;
            num = number;
        }
        else if (lines.back()[i] == '*')
        {
            op = product;
            num = number;
        }
        else
        {
            num = op(num, number);
        }
    }
    total += num;
    std::cout << total << '\n';
}