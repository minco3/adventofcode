#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>


int main() {
    std::fstream file("input.txt");
    std::string str;
    std::vector<int> buf;
    std::list<int> pos, neg, oxys, c02s;
    std::vector<std::vector<int>> v2;
    int c02 = 0, oxy = 0;

    while (!file.eof()) {
        std::getline(file, str);
        
        if (buf.size() == 0)
            buf.resize(str.size());

        for (int i=0; i<str.length(); i++) {
            buf[i] = str[i]-'0';
        }
        v2.push_back(buf);
    }

    for (int i=0; i<v2.size(); i++) {
        if (v2[i][0]) pos.push_back(i);
        else neg.push_back(i);
    }

    if (pos.size() >= neg.size()) {
        oxys = pos;
        c02s = neg;
    } else {
        oxys = neg;
        c02s = pos;
    }

    for (int i=1; i<v2[0].size(); i++) {
        pos.clear();
        neg.clear();
        if (oxys.size() != 1) {
            for (auto it = oxys.begin(); it != oxys.end(); it++) {
                if (v2[*it][i]) pos.push_back(*it);
                else neg.push_back(*it);
            }
            if (pos.size() >= neg.size()) {
                oxys = pos;
            } else {
                oxys = neg;
            }
        }
        pos.clear();
        neg.clear();
        if (c02s.size() != 1) {
            for (auto it = c02s.begin(); it != c02s.end(); it++) {                
                if (v2[*it][i]) pos.push_back(*it);
                else neg.push_back(*it);
            }
            if (pos.size() >= neg.size()) {
                c02s = neg;
            } else {
                c02s = pos;
            }
        }
    }
    for (int i=0, j=v2[oxys.front()].size()-1; i<v2[oxys.front()].size(); i++, j--) {
        oxy |= v2[oxys.front()][i] << j;
    }
    for (int i=0, j=v2[c02s.front()].size()-1; i<v2[c02s.front()].size(); i++, j--) {
        c02 |= v2[c02s.front()][i] << j;
    }
    std::cout << oxy << " " << c02 << "\n";
    std::cout << oxy*c02;
}