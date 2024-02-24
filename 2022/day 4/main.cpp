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
    string str, s1,s2,s3,s4;
    int var1 = 0, var2 = 0;

    while (!file.eof()) {
        getline(file, str);
        stringstream ss(str);
        getline(ss, s1, '-');
        getline(ss, s2, ',');
        getline(ss, s3, '-');
        getline(ss, s4);
        int a1 = stoi(s1), a2=stoi(s2), b1=stoi(s3), b2=stoi(s4);
        if ((a1>=b1 && a1<=b2) || (a2>=b1 && a2<=b2) || (b2>=a1 && b2<=a2) || (b2>=a1 && b2<=a2)) {
            var1++;
        }
    }



    std::cout << var1 << '\n' << var2;
}