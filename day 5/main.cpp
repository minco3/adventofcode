#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

const static int size = 1000;

struct Point {
    int x;
    int y;
    int convert() const {
        return y*size+x;
    }
    bool operator== (const Point& p) {
        return (x==p.x && y==p.y);
    }
    bool operator< (const Point& p) {
        return x+y<p.x+p.y;
    }
    bool operator> (const Point& p) {
        return convert()>p.convert();
    }
};

int main() {

    std::fstream file("input.txt");
    std::string str, a;
    std::unordered_multimap<int, int> map;
    int num, count = 0;

    Point first, second;
    while (!file.eof()) {
        std::getline(file, str);
        std::stringstream ss(str);
        ss >> num;
        first.x = num;
        ss.ignore(1);
        ss >> num;
        first.y = num;

        ss.ignore(4);
        
        ss >> num;
        second.x = num;
        ss.ignore(1);
        ss >> num;
        second.y = num;
        
        if (first>second) std::swap(first,second);

        if (first.x == second.x) { // vertical
            for (Point p=first; p.y<=second.y; p.y++) {
                map.insert({p.convert(), 0});
                if (map.count(p.convert()) == 2) count++;
            }
        } else if (first.y == second.y) { // horizontal
            for (Point p=first; p.x<=second.x; p.x++) {
                map.insert({p.convert(), 0});
                if (map.count(p.convert()) == 2) count++;
            }
        } else if (first.x < second.x) { // negative slope
            for (Point p=first; p.x<=second.x; p.x++, p.y++) {
                map.insert({p.convert(), 0});
                if (map.count(p.convert()) == 2) count++;
            }
        } else { // positive slope
            for (Point p=first; p.x>=second.x; p.x--, p.y++) {
                map.insert({p.convert(), 0});
                if (map.count(p.convert()) == 2) count++;
            }
        }
    }
    
    std::cout << count;
    return 0;
}