#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0;
    std::string str;
    constexpr bool debug = false;

    size_t ip = 0;
    std::array<size_t, 3> regs;
    std::vector<size_t> input;
    std::vector<size_t> output;

    std::getline(file, str);
    regs[0] = std::stoull(str.substr(12));
    std::getline(file, str);
    regs[1] = std::stoull(str.substr(12));
    std::getline(file, str);
    regs[2] = std::stoull(str.substr(12));
    std::getline(file, str);
    std::getline(file, str);
    str = str.substr(9);
    std::ranges::replace(str, ',', ' ');
    std::stringstream sstr(str);
    input = std::vector<size_t>(
        std::istream_iterator<size_t>(sstr), std::istream_iterator<size_t>());

    auto combo_op = [&](size_t operand) -> size_t
    { return operand <= 3 ? operand : regs[operand - 4]; };

    std::array<std::function<void(size_t)>, 8> ops = {
        [&](size_t operand)
        {
            regs[0] /= 1 << combo_op(operand);
            ip += 2;
        },
        [&](size_t operand)
        {
            regs[1] ^= operand;
            ip += 2;
        },
        [&](size_t operand)
        {
            regs[1] = combo_op(operand) & 0b111;
            ip += 2;
        },
        [&](size_t operand)
        {
            if (regs[0])
                ip = operand;
            else
                ip += 2;
        },
        [&](size_t operand)
        {
            regs[1] ^= regs[2];
            ip += 2;
        },
        [&](size_t operand)
        {
            output.push_back(combo_op(operand) & 0b111);
            ip += 2;
        },
        [&](size_t operand)
        {
            regs[1] = regs[0] / (1 << combo_op(operand));
            ip += 2;
        },
        [&](size_t operand)
        {
            regs[2] = regs[0] / (1 << combo_op(operand));
            ip += 2;
        },
    };

    while (ip < input.size())
    {
        ops[input[ip]](input[ip + 1]);
    }

    std::string out;
    for (auto n : output)
    {
        out += std::to_string(n);
        out += ",";
    }
    out.pop_back();

    size_t i = 0;
    while (true)
    {
        output.clear();
        ip = 0;
        regs = {count, 0, 0};
        while (ip < input.size())
        {
            ops[input[ip]](input[ip + 1]);
        }
        auto [it1, it2] = std::ranges::mismatch(
            input | std::views::reverse, output | std::views::reverse);
        if (it1 == input.rend())
            break;
        else if (auto d = std::distance(input.rbegin(), it1); d > i)
        {
            count <<= 3;
            i++;
        }

        count += 1;
    }

    std::println(
        "{}, {}, {}", out, count,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}