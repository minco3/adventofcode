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

using namespace std;

int main() {
    queue<unsigned long long> q;
    vector<unsigned long long> v;
    string str;
    unsigned long long var1 = 0, var2 = 0, sum = 0;
    unordered_set<unsigned long long> set;

    fstream file("input.txt");
    // fstream file("test.txt");

    long long counter = 1, step = 0, next = 20, line = 0;
    while (!file.eof()) {
        getline(file, str);
        if (str[0] == 'n') { // noop
            if (step == next) {
                v.push_back(counter * next); // step 20
                next += 40;
            }
            if (line%40==0) {
                line = 0;
                cout << "\n";
            }
            cout << (counter-1 <= line && line <= counter+1 ? '#' : '.');
            line++;
            step++;
        } else { // add
            if (step == next-1 || step == next-2) { // step 20
                v.push_back(counter * next);
                next += 40;
            }
            for (int i=0; i<2; i++) {
                if (line%40==0) {
                    line = 0;
                    cout << "\n";
                }
                cout << (counter-1 <= line && line <= counter+1 ? '#' : '.');
                line++;
                step++;
            }
            stringstream ss(str);
            int value;
            ss >> str >> value;
            counter+=value;

        }
    }

    for (auto x : v) {
        sum += x;
    }

    cout << sum;

    return 0;
}