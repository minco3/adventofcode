#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <algorithm>

int main()
{
    std::fstream f(SOURCE_DIR "/input.txt");
    std::string l;
    size_t count = 0;
    while (std::getline(f, l))
    {
        std::regex re("(\\d+)-(\\d+) (\\w): (\\w+)");
        std::smatch match;
        std::regex_match(l, match, re);

        int32_t min = std::stoi(match[1].str());
        int32_t max = std::stoi(match[2].str());
        char character = match[3].str()[0];
        auto password = match[4].str();

        // if (auto c = std::ranges::count(password, character); c >= min && c <= max)
        if (password[min-1] == character && password[max-1] != character || password[min-1] != character && password[max-1] == character)
        {
            count++;
        }
    }
    std::cout << count << '\n';
}