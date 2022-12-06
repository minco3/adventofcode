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
    
    fstream file("input.txt");
    // fstream file("test.txt");

    unordered_map<char, int> map;
    string str;

    getline(file, str);
    for (int i=0; i<13; i++)  {
        map.insert({str[i], 0});
        map.at(str[i])++;
    }

    for (int i=13; i<str.size(); i++) {
        map.insert({str[i], 0});
        map.at(str[i])++;
        if (map.size() == 14) {
            std::cout << i+1;
            break;
        }
        if (map.at(str[i-13]) == 1)  {
            map.erase(str[i-13]);
        } else {
            map.at(str[i-13])--;
        }
    }
}