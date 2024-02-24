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
    queue<int> q;
    vector<int> v;
    string str, str2, str3;
    int var1 = 0, var2 = 0;

    while (!file.eof()) {
        getline(file, str);
        getline(file, str2);
        getline(file, str3);
        bool found = false;
        for (auto x : str) {
            for (auto y : str2) {
                for (auto z : str3) {
                    if (x == y && x == z) {
                        found = true;
                        if (islower(x)) {
                        var1 += x-'a'+1;
                        } else {
                        var1 += x-'A'+27;
                        }
                    }
                    if (found) break;
                }
                if (found) break;
            }
            if (found) break;
        }
    }



    std::cout << var1 << '\n' << var2;
}