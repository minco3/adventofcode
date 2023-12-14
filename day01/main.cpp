#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string_view>
#include <array>

std::array<std::string, 9> words = {"one", "two", "three", "four", "five",
                                  "six", "seven", "eight", "nine"};

std::array<std::string, 9> numbers = {"1", "2", "3", "4", "5",
                                    "6", "7", "8", "9"};

void find_first(
    const std::string& str, const std::array<std::string, 9>& vals, int& number, ptrdiff_t& it)
{
    for (size_t i = 0; i < vals.size(); i++)
    {
        ptrdiff_t it2 = static_cast<ptrdiff_t>(str.find(vals.at(i)));
        if (it2 != std::string::npos && it2 < it)
        {
            number = i + 1;
            it = it2;
        }
    }
}
void find_last(
    const std::string& str, const std::array<std::string, 9>& vals, int& number, ptrdiff_t& it)
{
    for (size_t i = 0; i < vals.size(); i++)
    {
        ptrdiff_t it2 = static_cast<ptrdiff_t>(str.rfind(vals.at(i)));
        if (it2 != std::string::npos && it2 > it)
        {
            number = i + 1;
            it = it2;
        }
    }
}

int main()
{
    std::fstream file(SOURCE_DIR"/input.txt");
    std::string str;
    size_t sum = 0;
    while (!file.eof())
    {
        std::getline(file, str);
        int a = 0;
        int first_number, last_number, first;
        ptrdiff_t first_word = str.size() + 1;
        find_first(str, words, first_number, first_word);
        find_first(str, numbers, first_number, first_word);
        ptrdiff_t last_word = -1;
        find_last(str, words, last_number, last_word);
        find_last(str, numbers, last_number, last_word);
        a = (first_number * 10) + last_number;
        assert(a > 0 && a < 100);
        std::cout << a << std::endl;
        sum += a;
    }
    std::cout << sum << std::endl;
}