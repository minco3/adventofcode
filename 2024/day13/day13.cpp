#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <unordered_map>
#include <array>
#include <iostream>
#include <regex>
#include <vector>
#include <utility>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point &p) { return {k * p.x, k * p.y}; }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

bool contains(point pos, int64_t width, int64_t height)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
};

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/sample.txt");
    size_t count = 0, count2 = 0;
    std::string str;

    std::vector<std::pair<size_t, size_t>> map;
    std::vector<std::vector<uint8_t>> visited;
    point A, B, prize;

    while (std::getline(file, str))
    {
        if (str.size() == 0)
        {
            auto det = 
        }
        std::regex re(R"((Button \w|Prize): X.(\d+), Y.(\d+))");
        for (auto it = std::sregex_iterator(str.begin(), str.end(), re);
             it != std::sregex_iterator(); it++)
        {
            std::smatch match = *it;
            if (match[1] == "Button A")
                A = {std::stoll(match[2]), std::stoll(match[3])};   
            else if (match[1] == "Button B")
                B = {std::stoll(match[2]), std::stoll(match[3])};   
            else if (match[1] == "Button C")
                prize = {std::stoll(match[2]), std::stoll(match[3])};   
        }
    }

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
}