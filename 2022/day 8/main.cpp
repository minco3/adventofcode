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


int getScenicScore(const vector<vector<int>>& v, int x, int y) {
    int xl = 1,xr = 1 ,yl = 1,yr = 1;

    for (int i = x-1; i>0 && v[i][y] < v[x][y]; i--, xl++) {}// xl
    for (int i = x+1; i<v.size()-1 && v[i][y] < v[x][y]; i++, xr++) {}// xr
    for (int i = y-1; i>0 && v[x][i] < v[x][y]; i--, yl++) {} // yl
    for (int i = y+1; i<v[x].size()-1 && v[x][i] < v[x][y]; i++, yr++) {}// yr

    return xl*xr*yl*yr;
}

int main() {
    
    fstream file("input.txt");
    // fstream file("test.txt");

    unordered_map<char, int> map;
    vector<vector<int>> v;
    vector<vector<bool>> vb;
    string str;

    int treecount = 0;

    while(!file.eof()) {
        getline(file, str);
        v.resize(str.size());
        for (int i=0; i<str.size(); i++) {
            v[i].push_back(str[i]-48);
        }
    }

    vb.resize(v.size(), vector<bool>(v[0].size(), false));

    int x = 1, y = 0, maxScore = 0;

    treecount+=(v.size()-1)*2; // sides
    treecount+=(v[0].size()-1)*2;

    while (x < v.size()-1) { // top left to top right
        int highesttree = v[x][y];
        for (int i=1; highesttree != 9 && y+i < v[x].size()-1; i++) {
            if (v[x][y+i] > highesttree) {
                if (!vb[x][y+i]) {
                    int scenicScore = getScenicScore(v, x, y+i);
                    maxScore = scenicScore > maxScore ? scenicScore : maxScore;
                    vb[x][y+i] = true;
                    treecount++;
                }
                highesttree = v[x][y+i];
            }
        }
        x++;
    }
    y++;
    while (y < v[0].size()-1) { // top right to bottom right
        int highesttree = v[x][y];
        for (int i=1; highesttree != 9 && x-i>0; i++) {
            if (v[x-i][y] > highesttree) {
                if (!vb[x-i][y]) {
                    int scenicScore = getScenicScore(v, x-i, y);
                    maxScore = scenicScore > maxScore ? scenicScore : maxScore;
                    vb[x-i][y] = true;
                    treecount++;
                }
                highesttree = v[x-i][y];
            }
        }
        y++;
    }
    x--;
    while (x > 0) { // bottom right to bottom left
        int highesttree = v[x][y];
        for (int i=1; highesttree != 9 && y-i > 0; i++) {
            if (v[x][y-i] > highesttree) {
                if (!vb[x][y-i]) {
                    treecount++;
                    int scenicScore = getScenicScore(v, x, y-i);
                    maxScore = scenicScore > maxScore ? scenicScore : maxScore;
                    vb[x][y-i] = true;
                }
                highesttree = v[x][y-i];
            }
        }
        x--;
    }
    y--;
    while (y > 0) { // bottom left to top right
        int highesttree = v[x][y];
        for (int i=1; highesttree != 9 && x+i < v.size()-1; i++) {
            if (v[x+i][y] > highesttree) {
                if (!vb[x+i][y]) {
                    treecount++;
                    int scenicScore = getScenicScore(v, x+i, y);
                    maxScore = scenicScore > maxScore ? scenicScore : maxScore;
                    vb[x+i][y] = true;
                }
                highesttree = v[x+i][y];
            }
        }
        y--;
    }

    std::cout << treecount << " " << maxScore;

}