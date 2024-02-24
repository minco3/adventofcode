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

struct Point {
    int x=0,y=0;
};

int main() {
    queue<unsigned long long> q;
    vector<unsigned long long> v;
    string str;
    unsigned long long var1 = 0, var2 = 0;
    unordered_set<unsigned long long> set;
    Point nodes[10];

    // fstream file("input.txt");
    fstream file("test2.txt");

    while (!file.eof()) {
        getline(file, str);
        int step = stoi(str.substr(2));
        for (int i = 0; i<step; i++) {
            switch (str[0]) {
                case 'R':
                    nodes[0].x++;
                break;
                case 'L':
                    nodes[0].x--;
                break;
                case 'U':
                    nodes[0].y++;
                break;
                case 'D':
                    nodes[0].y--;
                break;
            }
            for (int j=1; j<10; j++) {
                if (abs(nodes[j].y - nodes[j-1].y) > 1 || abs(nodes[j].x - nodes[j-1].x) > 1) {
                    if (nodes[j].x == nodes[j-1].x) { // aligned on x axis
                        nodes[j].y += nodes[j-1].y > nodes[j].y ? nodes[j-1].y-nodes[j].y-1 : nodes[j-1].y-nodes[j].y+1; 
                    } else if (nodes[j].y == nodes[j-1].y) { // aligned on y axis
                        nodes[j].x += nodes[j-1].x > nodes[j].x ? nodes[j-1].x-nodes[j].x-1 : nodes[j-1].x-nodes[j].x+1; 
                    } else { // diag
                        int dx = nodes[j-1].x > nodes[j].x ? 1 : -1, dy = nodes[j-1].y > nodes[j].y ? 1 : -1;
                        nodes[j].x += dx;
                        nodes[j].y += dy;
                    }
                }
            }
            set.insert((unsigned int) nodes[9].x << 32 | (unsigned int) nodes[9].y );
        }
    }

    cout << set.size();
    return 0;
}