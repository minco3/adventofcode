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
    Point operator+ (const Point& rhs) const {
        return{x+rhs.x, y+rhs.y};
    }
};

struct cell {
    Point p;
    int weight, length;
    cell(int _weight, int _length = 0, Point _p = Point(), stack<Point> _path = stack<Point>()) : p(_p), weight(_weight), length(_length) {} 
    bool operator < (const cell& rhs) const {
        return weight+length < rhs.weight+rhs.length;
    }
    bool operator > (const cell& rhs) const {
        return weight+length > rhs.weight+rhs.length;
    }
};

int distance(Point a, Point b) {
    return (abs(a.x-b.x) + abs(a.y-b.y));
}

int weight(const Point& p, const Point& start, const Point& end) {
    return p.distance(start) + p.distance(end);
}

bool isValid(const Point& p, const Point& p2, const set<Point>& closed, const map<Point, int>& heightmap) {
    return heightmap.count(p2) && !closed.count(p2) && (heightmap.at(p2) < heightmap.at(p) || heightmap.at(p2) - heightmap.at(p) < 2);
}

int main() {
    map<Point, int> heightmap;
    multiset<cell, std::greater<>> startps;
    string str;
    Point end, start, p;
    vector<int> lengths;
    vector<Point> ps;

    vector<Point> dirs = {{1,0}, {0,1}, {-1,0}, {0,-1}};

    fstream file("input.txt");
    // fstream file("test.txt");

    int line = 0;
    while (!file.eof()) {
        getline(file, str);
        for (int i=0; i<str.length(); i++) {
            if (str[i] == 'E') {
                heightmap.insert({Point(i, line), 25});
                end = Point(i, line);
            } else if (str[i] == 'S') {
                heightmap.insert({Point(i, line), 0});
                ps.push_back(Point(i, line));
                // start = Point(i, line);
            } else {
                if (str[i] == 'a') ps.push_back(Point(i, line));
                heightmap.insert({Point(i, line), str[i]-'a'});
            }
        }
        line++;
    }

    for (const auto& p : ps) {
        startps.emplace(p.distance(end), 0, p);
    }

    int i = 0;

    for(auto& start : startps) {
        std::cout << '\n' << i << " / " << startps.size() << ' ';

        set<Point> closed;
        multiset<cell> open;
        map<Point, int> openPoints;

        open.insert(start);
        openPoints.emplace(start.p, 0);
        
        int a = 0;
        while (!open.empty()) {
            if (a%1000==0) {
                std::cout << 'A';
            }
            cell c = *open.begin();
            open.erase(open.begin());
            openPoints.erase(c.p);
            p = c.p;
            closed.insert(p);
            if (p == end) {
                lengths.push_back(c.length);
                cout << c.length;
                break;
            }
            
            
            for (auto& dir : dirs) {
                Point p2 = p + dir; 
                if (isValid(p, p2, closed, heightmap)) {
                    if (openPoints.count(p2)) {
                        if (openPoints.at(p2) <= c.length+1) continue;

                        auto itpair = open.equal_range(cell(openPoints.at(p2)));
                        for (auto it = itpair.first; it!=itpair.second; it++) {
                            if (it->p == p2) {
                                open.erase(it);
                            }
                        }
                        openPoints.erase(p2);
                    }
                    open.emplace(distance(p2, end), c.length+1, p2);
                    openPoints.insert({p2, c.length+1});
                }

            }
            a++;
        }
        i++;
    }

    make_heap(lengths.begin(), lengths.end(), std::greater<>{});

    std::cout << lengths[0];

    return 0;
}