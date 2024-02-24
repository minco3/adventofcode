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
    string str;
    int var1 = 0, var2 = 0;

    while (!file.eof()) {
        getline(file, str);
        bool found = false;
        for (int i=0; i<str.length()/2 && !found; i++) {
            for (int j=str.length()/2; j<str.length() && !found ; j++) {
                if (str[j] == str[i]) {
                    if (islower(str[i])) {
                        var1 += str[i]-'a'+1;
                    } else {
                        var1 += str[i]-'A'+27;
                    }
                    cout << var1 << '\n';
                    found = true;
                }
            }
        }
    }



    std::cout << var1 << '\n' << var2;
}