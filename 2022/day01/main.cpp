#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::fstream file("input.txt");

    std::vector<int> v;

    int total = 0;

    std::string str;
    
    while (!file.eof()) {
        std::getline(file, str);
        if (str.size() == 0) {
            v.push_back(total);
            total = 0;
        } else {
            total += stoi(str);
        }
    }

    v.push_back(total);


    total = 0;
    std::make_heap(v.begin(), v.end());
    total = v.front();
    std::pop_heap(v.begin(), v.end());
    total += v.front();
    v.pop_back();
    std::pop_heap(v.begin(), v.end());
    total += v.front();


    std::cout << total;
}