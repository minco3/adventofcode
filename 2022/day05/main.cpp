#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>

using namespace std;

int main() {
    // fstream file("test.txt");
    fstream file("input.txt");
    deque<int> q;
    vector<deque<char>> v;
    deque<char> dq;
    string str, ans, s1, s2, s3, s4, s5, s6;
    int var1 = 0, var2 = 0;

    while (!file.eof()) {
        getline(file, str);
        if (isdigit(str[1])) break;
        v.resize(str.length()/4+1);
        for (int i=0; i<str.length()/4+1; i++) {
            if (str[i*4+1] != ' ') {
                v[i].push_back(str[i*4+1]);
            }
        }
    }

    getline(file, str);

    while (!file.eof()) {
        getline(file, str);
        stringstream ss(str);
        ss >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;
        for (int i=0; i<stoi(s2); i++) {
            dq.push_front(v[stoi(s4)-1].front());
            v[stoi(s4)-1].pop_front();
        }
        while (!dq.empty()) {
            v[stoi(s6)-1].push_front(dq.front());
            dq.pop_front();
        }
    }

    for (int i=0; i<v.size(); i++) {
        ans += v[i].front();
    }


    std::cout << var1 << '\n' << var2 << '\n' << ans;
}