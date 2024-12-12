#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <unordered_map>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;
    std::getline(file, str);
    std::stringstream sstr(str);

    auto stones = std::vector<uint64_t>(std::istream_iterator<uint64_t>(sstr), std::istream_iterator<uint64_t>());

    std::unordered_map<uint64_t, size_t> list1;
    std::unordered_map<uint64_t, size_t> list2;

    for (auto stone : stones)
    {
        list2[stone]++;
    }

    size_t blink_count = 0, blink_count2 = 0;
    file >> blink_count >> blink_count2;

    auto step_n = [&](size_t steps)
    {
        for (size_t i = 0; i < steps; i++)
        {
            list1.clear();
            list1 = list2;
            list2.clear();
            for (auto [k, v] : list1)
            {
                if (k == 0)
                {
                    list2[1] += v;
                }
                else if (uint64_t n = static_cast<uint64_t>(std::floor(std::log10(k) + 1)); n % 2 == 0)
                {
                    uint64_t pow = static_cast<uint64_t>(std::pow(10, n / 2));
                    list2[k % pow] += v;
                    list2[k / pow] += v;
                }
                else
                {
                    list2[k * 2024] += v;
                }
            }
        }
    };

    step_n(blink_count);

    for (auto [k, v] : list2)
    {
        count += v;
    }

    step_n(blink_count2);

    for (auto [k, v] : list2)
    {
        count2 += v;
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}