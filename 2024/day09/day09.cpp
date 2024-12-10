#include <fstream>
#include <print>
#include <sstream>
#include <chrono>
#include <set>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <cassert>

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

constexpr const size_t empty_id = std::numeric_limits<size_t>::max();

struct record
{
    size_t size, id;
    bool empty() { return id == empty_id; }
};

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::fstream file(SOURCE_DIR "/input.txt");
    std::string str;
    size_t count = 0, count2 = 0;
    std::vector<size_t> ids;
    std::vector<record> records;

    while (std::getline(file, str))
    {
        for (size_t i = 0; i < str.size(); i++)
        {
            char size = str[i] - '0';
            if (size)
            {
                records.emplace_back(size, (i % 2 ? empty_id : i / 2));
            }
            for (size_t j = 0; j < size; j++)
            {
                ids.push_back(i % 2 == 0 ? i / 2 : empty_id);
            }
        }
    }

    auto l = ids.begin();
    auto r = ids.end() - 1; // TODO: swap with riter

    for (size_t entry = 0; l <= r;)
    {
        if (*l != empty_id)
        {
            // std::cout << *l << ", ";
            count += entry++ * *l++;
        }
        else if (*r != empty_id)
        {
            // std::cout << *r << ", ";
            count += entry++ * *r--;
            l++;
        }
        else
        {
            r--;
        }
    }

    auto rr = records.rbegin();
    rr = std::find_if(rr, records.rend(), [](record r)
                      { return !r.empty(); });
    for (size_t i = (rr->id) + 1; i > 0; i--)
    {
        rr = std::find_if(records.rbegin(), records.rend(), [i](record r)
                          { return r.id == i - 1; });
        auto lr = std::find_if(records.begin(), rr.base(), [i, size = rr->size](record r)
                               { return r.empty() && r.size >= size; });
        if (lr != rr.base())
        {
            size_t temp = lr->size;
            lr->size = rr->size;
            lr->id = rr->id;
            rr->id = empty_id;
            if (temp > rr->size)
            {
                records.insert(lr + 1, {temp - rr->size, empty_id});
            }
        }
    }

    size_t entry = 0;
    for (auto r : records)
    {
        if (r.empty())
            entry += r.size;
        for (size_t j = 0; j < r.size; j++)
        {
            std::cout << (r.empty() ? "." : std::to_string(r.id));
            count2 += entry++ * r.id;
        }
    }
    // std::cout << '\n';

    std::println("{}, {}, {}", count, count2, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1));
    // assert(count == 6390180901651);
}