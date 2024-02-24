#include <fstream>
#include <iostream>
#include <vector>
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
    unordered_multiset<std::string> set;
    vector<vector<bool>> v;
    int x, y;

    unsigned long long size = 0;

    while (!file.eof()) {
        getline(file, str);
        if (str == "") break;
        stringstream ss(str);
        // cout << ss.str() << '\n';
        getline(ss, str2, ',');
        x = stoi(str2);
        getline(ss, str2);
        y = stoi(str2);

        if (x >= v.size())
            v.resize(x+1, vector<bool>(v.size() ? v[0].size() : y, false));

        if (y >= v[x].size()) {
            for (int i=0; i<v.size(); i++) {
                v[i].resize(y+1, false);
            }
        }
        v[x][y] = true;
        size++;
    }

    while (!file.eof()) {
        getline(file, str);
        int a = stoi(str.substr(13));
        switch (str[11]) {
            case 'x':
                for (int i=0; i<a; i++) {
                    for (int j=0; j<v[i].size(); j++) {
                        if (v[i][j] && v[a*2-i][j]) {
                            size--;
                        } else if (v[a*2-i][j]) {
                            v[i][j] = true;
                        }
                    }
                }
                v.resize(a);
            break;
            case 'y':
                for (vector<bool>& line : v) {
                    for (int i=0; i<a; i++) {
                        if (line[a*2-i]) {
                            if (line[i]) size--;
                            else line[i] = true;
                        }
                    }
                    line.resize(a);
                }
            break;
        }

        std::cout << size << '\n';
    }


    for (int y=0; y<v[0].size(); y++) {
        for (int x=0; x<v.size(); x++) {
            std::cout << (v[x][y] ? '#' : '.');
        }
        std::cout << '\n';
    }

    return 0;
}