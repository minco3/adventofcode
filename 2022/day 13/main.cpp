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

struct nestedlist {
    nestedlist(int n = -1) : a(n) {} 
    int a;
    vector<nestedlist*> b;
};

vector<nestedlist*> parselist(string str, int &j) {
    vector<nestedlist*> res;
    for (int i=1; i<str.size()-1; i++) {
        if (str[i] == '[') {
            nestedlist* l = new nestedlist;
            l->b = parselist(str.substr(i), j);
            i+=j;
            res.push_back(l);
        } else if (isdigit(str[i])) {
            res.push_back(new nestedlist(str[i]-'0'));
        } else if (str[i] == ']') {
            j = i;
            break;
        }
    }
    return res; 
}

vector<nestedlist*> tolist(int x) {
    vector<nestedlist*> res;
    if (x >=0 ) res.push_back(new nestedlist(x));
    return res;
}

bool comparelists(const vector<nestedlist*> &lhs, int rhs) {
    if (lhs[0]->b.empty()) {
        return lhs[0]->a < rhs;
    } else {
        return comparelists(lhs[0]->b, rhs);
    }
}

bool comparelists(int lhs, const vector<nestedlist*> &rhs) {
    if (rhs[0]->b.empty()) {
        if (lhs < rhs[0]->a) {
            return true;
        } else if (lhs == rhs[0]->a) {
            return rhs.size()>1;
        }
    } else {
        return comparelists(lhs, rhs[0]->b);
    }
}

bool comparelists(const vector<nestedlist*> &lhs, const vector<nestedlist*> &rhs) {
    for (int i=0; i<lhs.size() && i<rhs.size(); i++) {
        if (lhs[i]->b.empty() && rhs[i]->b.empty()) {
            if (lhs[i]->a > rhs[i]->a) {
                return false;
            } else if (lhs[i]->a < rhs[i]->a) {
                return true;
            } 
        } else if (!lhs[i]->b.empty() && !rhs[i]->b.empty()) {
            if (!comparelists(lhs[i]->b, rhs[i]->b)) return false;
        } else if (lhs[i]->b.empty()) {
            if (!comparelists(tolist(lhs[i]->a), rhs[i]->b)) return false;
        } else {
            if (!comparelists(lhs[i]->b, tolist(rhs[i]->a))) return false;
        }
    }
    return lhs.size() <= rhs.size();
}

int main() {
    vector<vector<int>> map;
    queue<unsigned long long> q;
    vector<int> line;
    vector<int> v;
    string str, lhss, rhss;
    unsigned long long var1 = 0, var2 = 1;

    fstream file("input.txt");
    // fstream file("test.txt");

    while (!file.eof()) {
        bool order = true;
        getline(file, lhss);
        getline(file, rhss);
        int i = 0;
        auto lhs = parselist(lhss, i);
        auto rhs = parselist(rhss, i);

        order = comparelists(lhs, rhs);

        if (order) {
            var1 += var2;
        }
        var2++;
        getline(file, str);
    }    

    cout << var1;

    return 0;
}