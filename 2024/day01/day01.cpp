#include <algorithm>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <unordered_map>

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    std::vector<int> l, r;
    while (std::getline(file, str))
    {
        std::stringstream sstr(str);
        int a, b;
        sstr >> a >> b;
        l.push_back(a);
        r.push_back(b);
    }

    std::ranges::sort(l);
    std::ranges::sort(r);

    size_t sum = 0;
    for (auto [l, r] : std::views::zip(l, r))
    {
        sum += abs(l - r);
    }
    std::println("{}", sum);

    // part 2
    std::unordered_map<int, size_t> map;
    sum = 0;
    for (auto n : l)
    {
        if (!map.contains(n))
        {
            map.insert({n, std::ranges::count(r, n)});
        }
        sum += n * map[n];
    }
    std::println("{}", sum);
}