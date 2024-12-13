#include <chrono>
#include <cmath>
#include <fstream>
#include <print>
#include <regex>

struct point
{
    int64_t x, y;
    point operator+(point r) { return {x + r.x, y + r.y}; }
    point operator-(point r) { return {x - r.x, y - r.y}; }
    friend point operator*(int64_t k, const point& p)
    {
        return {k * p.x, k * p.y};
    }
    bool operator==(point r) { return x == r.x && y == r.y; }
    bool operator<(const point r) const { return x != r.x ? x < r.x : y < r.y; }
};

bool is_int(double v)
{
    double int_part = 0.0;
    return std::abs(int_part - std::modf(v, &int_part)) < 0.001;
}

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    size_t count = 0, count2 = 0;
    std::string str;

    point A, B, prize;

    while (std::getline(file, str))
    {
        const std::regex re(R"((Button \w|Prize): X.(\d+), Y.(\d+))");
        std::smatch match;
        if (std::regex_match(str, match, re))
        {
            if (match[1] == "Button A")
                A = {std::stoll(match[2]), std::stoll(match[3])};
            else if (match[1] == "Button B")
                B = {std::stoll(match[2]), std::stoll(match[3])};
            else if (match[1] == "Prize")
            {
                prize = {std::stoll(match[2]), std::stoll(match[3])};
                auto det = static_cast<double>(A.x * B.y - B.x * A.y);
                if (std::abs(det) < 0.001)
                    break;

                auto A_count = (B.y * prize.x - B.x * prize.y) / det;
                auto B_count = (-A.y * prize.x + A.x * prize.y) / det;
                if (A_count <= 100.0 && B_count <= 100.0 && is_int(A_count) &&
                    is_int(B_count))
                {
                    count += A_count * 3 + B_count;
                }
                prize = prize + point(10000000000000, 10000000000000);
                auto A_count2 = (B.y * prize.x - B.x * prize.y) / det;
                auto B_count2 = (-A.y * prize.x + A.x * prize.y) / det;
                if (is_int(A_count2) && is_int(B_count2))
                {
                    count2 += A_count2 * 3 + B_count2;
                }
            }
        }
    }

    std::println(
        "{}, {}, {}", count, count2,
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - t1));
}