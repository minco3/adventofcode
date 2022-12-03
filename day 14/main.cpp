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

    fstream file("testA.txt");
    string str, str2;
    stringstream ss;
    unordered_map<string, unsigned long> map, map2;
    unordered_map<char, unsigned long> map3;
    unordered_map<string, char> rules;

    static const int steps = 10;

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
        for (pair<string, char> p : rules) {
            auto it = map.find(p.first);
            if (it != map.end() && it->second != 0) { // rule exists
                auto it2 = map2.find(p.first);
                map2.insert({"" + p.first[0] + p.second, it->second}); // insert lhs + result
                map2.insert({"" + p.second + p.first[1], it->second}); // insert result + rhs
                it2->second = 0; // clear lhs+rhs
                map3.insert({p.second, 0});
                map3.at(p.second) += it->second;
            }
        }
        std::cout << "step " << i << '\n';
    }

    char maxchar, minchar;
    unsigned long maxint = 0, minint = -1;

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