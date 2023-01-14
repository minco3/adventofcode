#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct Point {
    int x,y;
Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    int distance(const Point& b) const {
        return (abs(x-b.x) + abs(y-b.y));
    }
    unsigned long long compress() const {
        return (unsigned long long) x << 32 | (unsigned int) y;
    }
    bool operator < (const Point& rhs) const {
        return compress() < rhs.compress();
    }
    bool operator == (const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};
struct cell {
    Point p;
    int weight, length;
    cell(Point _p = Point(), int _weight = 0, int _length = 0) : p(_p), weight(_weight), length(_length) {} 
    bool operator < (const cell& rhs) const {
        return weight > rhs.weight;
    }
    
};

int distance(Point a, Point b) {
    return (abs(a.x-b.x) + abs(a.y-b.y));
}

int weight(const Point& p, const Point& start, const Point& end) {
    return p.distance(start) + p.distance(end);
}


int main() {
    map<Point, int> heightmap;
    set<cell> closed;
    queue<unsigned long long> q;
    vector<int> v;
    string str;
    priority_queue<cell> pq; 
    unsigned long long var1 = -1, var2 = 0;
    Point end, start, p;

    fstream file("input.txt");
    // fstream file("test.txt");

    int line = 0;
    while (!file.eof()) {
        getline(file, str);
        for (int i=0; i<str.length(); i++) {
            if (str[i] == 'E') {
                heightmap.insert({Point(i, line), 'z'-'a'});
                end = Point(i, line);
            } else if (str[i] == 'S') {
                heightmap.insert({Point(i, line), 0});
                start = Point(i, line);
            } else {
                heightmap.insert({Point(i, line), str[i]-'a'});
            }
        }
        line++;
    }

    pq.emplace(start, weight(start, start, end), 0);
    int abc = 0;

    while (true) {
        if (abc%100 == 0) {
            
        }
        cell c = pq.top();
        pq.pop();
        p = c.p;
        closed.insert(c);
        if (p == end) {
            cout << c.length;
            break;
        }
        Point p2 = Point(p.x-1, p.y);
        if (heightmap.count(p2) && !closed.count(cell(p2)) && (heightmap.at(p2) < heightmap.at(p) || heightmap.at(p2) - heightmap.at(p) < 2)) {
            Point p2(p.x-1, p.y);
            pq.emplace(p2, weight(p2, start, end), c.length+1);
        }
        p2 = Point(p.x+1, p.y);
        if (heightmap.count(p2) && !closed.count(cell(p2)) && (heightmap.at(p2) < heightmap.at(p) || heightmap.at(p2) - heightmap.at(p) < 2)) {
            pq.emplace(p2, weight(p2, start, end), c.length+1);
        }
        p2 = Point(p.x, p.y-1);
        if (heightmap.count(p2) && !closed.count(cell(p2)) && (heightmap.at(p2) < heightmap.at(p) || heightmap.at(p2) - heightmap.at(p) < 2)) {
            pq.emplace(p2, weight(p2, start, end), c.length+1);
        }
        p2 = Point(p.x, p.y+1);
        if (heightmap.count(p2) && !closed.count(cell(p2)) && (heightmap.at(p2) < heightmap.at(p) || heightmap.at(p2) - heightmap.at(p) < 2)) {
            pq.emplace(p2, weight(p2, start, end), c.length+1);
        }
        abc++;
    }
    return 0;
}