#include <iostream>
#include <fstream>
#include <unordered_set>

int main()
{
    constexpr const uint64_t target = 2020ull;
    std::fstream f(SOURCE_DIR"/input.txt");
    std::unordered_set<uint64_t> s;
    std::string l;
    while (std::getline(f, l))
    {
        auto n = std::stoull(l);
        auto target2 = target - n;
        for (auto n2 : s)
        {
            if (s.contains(target2 - n2))
            {
                std::cout << n * n2 * (target2 - n2) << '\n';            
                return 0;
            }
        }
        s.insert(n);
    }
    return 1;
}