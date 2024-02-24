#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>

using namespace std;

int main() {

    fstream file("input.txt");
    string str, str2;
    stringstream ss;
    unordered_map<string, unsigned long long> map, map2;
    unordered_map<char, unsigned long long> map3;
    unordered_map<string, char> rules;

    static const int steps = 40;

    unsigned long long size = 0;

    getline(file, str);
    for (int i=0; i<str.size()-1; i++) {
        map2.insert({str.substr(i,2), 0}).first->second++;
    }

    for (char c : str) {
        map3.insert({c, 0}).first->second++;
    }

    getline(file, str);

    while (!file.eof()) {
        getline(file, str);
        rules.insert({str.substr(0,2), str[6]});
    }

    for (int i=0; i<steps; i++) {
        map = map2;
        for (const pair<string, unsigned long long> &p : map) {
            if (p.second == 0) continue;
            auto it = rules.find(p.first);
            if (it == rules.end()) continue; // rule doesnt exist

            
            map2.at(it->first) -= p.second; // clear lhs+rhs
            auto res = map2.insert({string() + it->first[0] + it->second, 0}); // insert lhs + result
            res.first->second += p.second;
            res = map2.insert({string() + it->second + it->first[1], 0}); // insert result + rhs
            res.first->second += p.second;
            auto res2 = map3.insert({it->second, 0});
            res2.first->second += p.second;

        }
        std::cout << "step " << i << '\n';
    }

    char maxchar, minchar;
    unsigned long long maxint = 0, minint = -1;

    for (auto pair : map3) {
        if (pair.second > maxint) {
            maxchar = pair.first;
            maxint = pair.second;
        }
        if (pair.second < minint) {
            minchar = pair.first;
            minint = pair.second;
        }
    }
    std::cout << maxchar << " " << maxint << " " << minchar << " " << minint << " " << maxint-minint << " " << map.size();

    return 0;
}