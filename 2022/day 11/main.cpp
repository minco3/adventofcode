#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct monkey {
    vector<unsigned long long> items;
    char operation;
    int val, test, iftrue, iffalse, score = 0;
};

int main() {
    queue<unsigned long long> q;
    vector<unsigned long long> v;
    string str;
    const int steps = 10000;
    int mod = 1;
    unsigned long long var1 = 0, var2 = 0;
    vector<monkey> monkeys;
    monkey m;

    fstream file("input.txt");
    // fstream file("test.txt");

    while (!file.eof()) {
        getline(file, str);
        if (str.empty()) {
            monkeys.push_back(m);
        } else if (str[2] == 'S') {
            m.items.clear();
            stringstream ss(str.substr(18));
            while (!ss.eof()) {
                int val;
                getline(ss, str, ',');
                m.items.push_back(stoi(str));
            }
        } else if (str[2] == 'O') {
            m.operation = str[23];
            if (str[25] == 'o') {
                m.val = -1;
            } else {
                m.val = stoi(str.substr(25));
            }
        } else if (str[2] == 'T') {
            m.test = stoi(str.substr(21));
        } else if (str[4] == 'I' && str[7] == 't') {
            m.iftrue = stoi(str.substr(29));
        } else if (str[4] == 'I' && str[7] == 'f') {
            m.iffalse = stoi(str.substr(30));
        }
    }
    monkeys.push_back(m);

    for (auto m : monkeys) {
        mod *= m.test;
    }

    for (int i=0; i<steps; i++) {
        for (auto &m : monkeys) {
            m.score += m.items.size();
            for (auto item : m.items) {
                if (m.operation == '*') {
                    item *= m.val==-1 ? item : m.val;
                } else {
                    item += m.val==-1 ? item : m.val;
                }

                item %= mod;

                if (item%m.test == 0) {
                    monkeys[m.iftrue].items.push_back(item);
                } else {
                    monkeys[m.iffalse].items.push_back(item);
                }
            }
            m.items.clear();
        }
    }

    for (auto m : monkeys) {
        v.push_back(m.score);
    }



    make_heap(v.begin(), v.end());
    var1=v.front();
    pop_heap(v.begin(), v.end());
    var1*=v.front();

    cout << var1;

    return 0;
}