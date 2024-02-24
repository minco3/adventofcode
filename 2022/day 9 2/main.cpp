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
    unordered_set<string> set;
    Point nodes[10];

    // fstream file("input.txt");
    fstream file("test2.txt");

    std::cout<<set.max_size();

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
                if (abs(nodes[i].y - nodes[i-1].y) > 1 || abs(nodes[i].x - nodes[i-1].x) > 1) {
                    if (nodes[i-1].x == nodes[i].x) { // aligned on x axis
                        nodes[i].y += nodes[i-1].y > nodes[i].y ? nodes[i-1].y-nodes[i].y-1 : nodes[i-1].y-nodes[i].y+1; 
                    } else if (nodes[i-1].y == nodes[i].y) { // aligned on y axis
                        nodes[i].x += nodes[i-1].x > nodes[i].x ? nodes[i-1].x-nodes[i].x-1 : nodes[i-1].x-nodes[i].x+1; 
                    } else { // diag
                        int dx = nodes[i-1].x > nodes[i].x ? 1 : -1, dy = nodes[i-1].y > nodes[i].y ? 1 : -1;
                        nodes[i].x += dx;
                        nodes[i].y += dy;
                    }
                }
            }
            set.insert(string() + to_string(nodes[9].x) + ',' + to_string(nodes[9].y));
        }

    }

    cout << set.size();
    return 0;
}