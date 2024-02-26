#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>


int main() {

    std::fstream file("input.txt");
    std::vector<std::string> v; // v[y][x]
    std::stack<char> s;
    std::string str;
    std::vector<unsigned long long> scores;

    unsigned long long total = 0, score;
    

    while (!file.eof()) {
        file >> str;
        v.push_back(str);
    }

    for (std::string row : v) {
        score = 0;
        s = std::stack<char>();
        for (char c : row) {
            switch (c) {
            case ')':
                if (s.top() != '(') {
                    std::cout << "expected ( but found " << s.top() << '\n';
                    score += 3;
                } 
                s.pop();
            break;
            case ']':
                if (s.top() != '[') {
                    std::cout << "expected [ but found " << s.top() << '\n';
                    score += 57;
                } 
                s.pop();
            break;
            case '}':
                if (s.top() != '{') {
                    std::cout << "expected { but found " << s.top() << '\n';
                    score += 1197;
                } 
                s.pop();
            break;
            case '>':
                if (s.top() != '<') {
                    std::cout << "expected < but found " << s.top() << '\n';
                    score += 25137;
                } 
                s.pop();
            break;
            default:
                s.push(c);
            }
        }
        if (s.empty() || score) continue;

        score = 0;
        while (!s.empty()) {
            score *=5;
            switch (s.top()) {
                case '(':
                    score += 1;
                break;
                case '[':
                    score += 2;
                break;
                case '{':
                    score += 3;
                break;
                case '<':
                    score += 4;
                break;
            }
            s.pop();
        }
        scores.push_back(score);
    }


    std::sort(scores.begin(), scores.end());

    std::cout << scores[scores.size()/2];
    
    std::cout << "\n";

    std::cout << total;

    return 0;
}