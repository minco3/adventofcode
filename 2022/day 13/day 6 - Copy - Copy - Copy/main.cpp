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
#include <algorithm>

using namespace std;

struct Point {
    int x, y; 
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator == (const Point& rhs) const {
        return x==rhs.x && y==rhs.y;
    }
    Point operator + (const Point& rhs) const {
        return Point(x+rhs.x, y+rhs.y);
    }
};

struct cell {
    int length, weight;
    Point p;
    cell (Point _p = Point(), int _length = 0, int _weight = 0) : p(_p), length(_length), weight(_weight) {}
    bool operator < (const cell &rhs) const {
        return length+weight > rhs.length+rhs.weight;
    }
    int score() {
        return length+weight;
    }
};

int distance(const Point& lhs, const Point& rhs) {
    return (abs(lhs.x - rhs.x) + abs(lhs.y-rhs.y));
}

cell* findNode(vector<cell>& open, Point p) {
    for (auto it = open.begin(); it != open.end(); it++) {
        if (it->p == p) {
            return &(*it);
        }
    }
    return nullptr;
}

bool isvalid (const vector<vector<int>>& map, vector<cell>& closed, const Point &p1, const Point &p2) {
    return (p2.x >= 0 && p2.x < map.size() && p2.y >= 0 && p2.y < map[0].size() && findNode(closed, p2) == nullptr && map[p2.x][p2.y] - map[p1.x][p1.y] < 2);
}


int astar(const vector<vector<int>> &map, const Point& start, const Point& end) {
    Point p;
    vector<cell> open, closed;
    vector<Point> directions = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    int a =0;
    open.emplace_back(start, 0, distance(start, end));
    while (true) {
        if (a%100==0) {
            cout << 'y';
        }
        auto current_it = open.begin();
        cell c = *current_it;
        for (auto it = open.begin(); it != open.end(); it++) {
            cell c2 = *it;
            if (c2.score() <= c.score()) {
                c = c2;
                current_it = it;
            }
        }
        p = c.p;

        if (p == end) {
            return c.length;
        }

        closed.push_back(c);
        open.erase(current_it);

        for (int i=0; i<directions.size(); i++) {
            Point p2 = p+directions[i];
            if (!isvalid(map, closed, p, p2)) {
                continue;
            }

            auto it = findNode(open, p2);
            if (it == nullptr) {
                open.emplace_back(p2, c.length+1, distance(p2, end));
            } else {
                it->length = c.length+1;
            }
        }
        a++;
    }
}

int main() {
    vector<vector<int>> map;
    queue<unsigned long long> q;
    vector<int> line;
    vector<int> v;
    string str;
    unsigned long long var1 = -1, var2 = 0;
    Point end, start;

    fstream file("input.txt");
    // fstream file("test.txt");
    while (!file.eof()) {
        getline(file, str);
        map.resize(str.length());
        for (int i=0; i<str.length(); i++) {
            if (str[i] == 'E') {
                end.x = i; 
                end.y = map[i].size();
                map[i].push_back('z'-'a');
            } else if (str[i] == 'S') {
                start.x = i;
                start.y = map[i].size();
                map[i].push_back(0);
            } else {
                map[i].push_back(str[i]-'a');
            }
        }
    }    
    // state.resize(map.size());
    // for (auto &v : state ) {
    //     v.resize(map[0].size(), -2);
    // }

    cout << astar(map, start, end);

    return 0;
}