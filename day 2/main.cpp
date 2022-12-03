#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

int main() {
    std::fstream file("input.txt");
    std::queue<int> q;
    std::vector<int> v;
    std::string str;
    int var1 = 0, var2 = 0;
    char c, c2;
    while (!file.eof()) {
        file >> str;
        c = str[0];
        file >> str;
        c2 = str[0];
        c-='A';
        c2-='X';
        var1 += c2 + 1;
        switch (c-c2) {
            case 0: // draw
            var1 += 3;
            break;
            case 1: // lose
            case -2: // lose
            break;
            case -1: // win
            case 2: // win
            var1 += 6;
            break;
        }
        switch (c2) {
            case 0: // lose
            var2 += (c+2)%3+1;
            std::cout << (c+2)%3+1 << '\n';
            break;
            case 1: // tie
            var2 += 3;
            var2 += c+1;
            std::cout << (c+1) << '\n';
            break;
            case 2: // win
            var2 += 6;
            var2 += (c+1)%3+1;
            std::cout << (c+1)%3+1 << '\n';
            break;
        }
    }


    /*
    A X rock
    B Y paper
    C Z scissors
    */





    std::cout << var1 << '\n' << var2;
}