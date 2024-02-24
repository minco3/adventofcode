#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

void print(const std::vector<std::vector<int>>& v) {
    for (auto row : v) {
        for (auto c : row) {
                std::cout << c;
            }
            std::cout << '\n';
        }
    std::cout << "\n\n";
}

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
    bool operator== (const Point& LHS) {
        return LHS.x == x && LHS.y == y; 
    }
};

int flash(std::vector<std::vector<int>>& v, Point p) {
    int flashedCount = 1; // inital point;

    v[p.y][p.x] = 0;

    int startY = p.y != 0 ? p.y-1 : p.y, 
    startX = p.x != 0 ? p.x-1 : p.x,
    endY = p.y != v.size()-1 ? p.y+1 : p.y,
    endX = p.x != v[0].size()-1 ? p.x+1 : p.x;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            if (v[y][x] == 0) continue; // initial point or other flashed point
            v[y][x] += 1;
            if (v[y][x] == 10) flashedCount+=flash(v, {x,y}); 
        }
    }
    return flashedCount;
}

int main() {

    std::fstream file("input.txt");
    std::vector<std::vector<int>> v; //v[y][x]
    std::queue<Point> flashes;

    std::string str;

    unsigned long long flashCount = 0;
    static const int steps = 1000;

    while (!file.eof()) {
        std::vector<int> vec;
        file >> str;
        for (char c : str) {
            vec.push_back(c-'0');
        }
        v.push_back(vec);
    }

    for (int i=0; i<steps; i++) {
        int stepFlashes = 0;
        for (int y=0; y<v.size(); y++) {
            for (int x=0; x<v[0].size(); x++) {
                v[y][x] += 1;
                if (v[y][x] == 10) {
                    flashes.emplace(x,y);
                }
            }
        }

        while (!flashes.empty()) {
            stepFlashes += flash(v, flashes.front());
            flashes.pop();
        }
        // print(v);
        if (stepFlashes == v.size()*v[0].size()) {
            std::cout << "step " << i+1;
            return 0; 
        }
        flashCount += stepFlashes;
    }

    
    std::cout << flashCount;

    return 0;
}