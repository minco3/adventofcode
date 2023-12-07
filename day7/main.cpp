#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <array>

using hand_t = std::pair<std::string, size_t>;

enum score_t
{
    HighCard,
    OnePair,
    TwoPair,
    ThreePair,
    FullHouse,
    FourPair,
    FivePair
};

const std::array<char, 13> rank = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};

size_t find_rank(char& c)
{
    return static_cast<size_t>(std::distance(rank.begin(), std::find(rank.begin(), rank.end(), c)));
}

score_t score(hand_t& hand)
{
    std::vector<std::pair<char, size_t>> oc;
    size_t j = 0;
    for (char c : hand.first)
    {   
        if (c == 'J')
            j++;
        if (auto it = std::find_if(
                oc.begin(), oc.end(),
                [c](std::pair<char, size_t> o) { return o.first == c; });
            it != oc.end())
            it->second++;
        else
            oc.push_back({c, 1});
    }
    std::sort(
        oc.begin(), oc.end(),
        [](std::pair<char, size_t> lhs, std::pair<char, size_t> rhs)
        { return lhs.second > rhs.second; });
    if (oc.at(0).first == 'J')
    {
        j=0;
        if (oc.size() > 1)
        {
            oc.at(0).second+=oc.at(1).second;
        }
    }
    switch (oc.at(0).second+j)
    {
    case 5:
        return FivePair;
    case 4:
        return FourPair;
    case 3:
        if (oc.at(1).second == 2)
            return FullHouse;
        else
            return ThreePair;
    case 2:
        if (oc.at(1).second == 2)
            return TwoPair;
        else
            return OnePair;
    default:
        return HighCard;
    }
}

int main()
{
    std::fstream file(SOURCE_DIR "/input.txt");
    std::vector<hand_t> hands;
    std::string str, hand;
    size_t sum = 0, bid;
    
    // hand_t h ={"JJJJJ", 123};
    // std::cout << score(h);
    while (std::getline(file, str))
    {
        std::stringstream sstr(str);
        sstr >> hand >> bid;
        hands.push_back({hand, bid});
    }
    std::sort(
        hands.begin(), hands.end(),
        [](hand_t& lhs, hand_t& rhs)
        {
            size_t ls = static_cast<size_t>(score(lhs)),
                   rs = static_cast<size_t>(score(rhs));
            if (ls != rs)
            {
                return ls > rs;
            }
            else
            {
                for (size_t i = 0; i < lhs.first.size(); i++)
                {
                    if (lhs.first[i] != rhs.first[i])
                    {
                        return find_rank(lhs.first[i]) < find_rank(rhs.first[i]);
                    }
                }
            }
        });
    for (size_t i = 0; i < hands.size(); i++)
    {
        auto& h = hands.at(i);
        sum += (hands.size() - i) * h.second;
        std::cout << h.first << std::endl;
    }
    std::cout << sum << std::endl;
}