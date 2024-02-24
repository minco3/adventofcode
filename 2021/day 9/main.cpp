#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

int DFS(const std::vector<std::string>& data, std::vector<std::vector<bool>>& visited, Point p) {
    int size = 1; // current point is valid
    visited[p.y][p.x] = true;
    if (p.x != 0 && data[p.y][p.x] < data[p.y][p.x-1] && !visited[p.y][p.x-1] && data[p.y][p.x-1] != '9') {
        size += DFS(data, visited, {p.x-1, p.y});
    }
    if (p.x != data[0].size()-1 && data[p.y][p.x] < data[p.y][p.x+1] && !visited[p.y][p.x+1] && data[p.y][p.x+1] != '9') {
        size += DFS(data, visited, {p.x+1, p.y});
    }
    if (p.y != 0 && data[p.y][p.x] < data[p.y-1][p.x] && !visited[p.y-1][p.x] && data[p.y-1][p.x] != '9') {
        size += DFS(data, visited, {p.x, p.y-1});
    }
    if (p.y != data.size()-1 && data[p.y][p.x] < data[p.y+1][p.x] && !visited[p.y+1][p.x] && data[p.y+1][p.x] != '9') {
        size += DFS(data, visited, {p.x, p.y+1});
    }
    return size;
}

int main() {

    std::fstream file("input.txt");
    std::vector<std::string> v; // v[y][x]
    std::string str;
    std::queue<Point> pq;
    std::vector<int> heap;

    unsigned int total;

    while (!file.eof()) {
        file >> str;
        v.push_back(str);
    }

    std::vector<std::vector<bool>> visited(v.size(), std::vector<bool>(v[0].size(), false));

    for (int y=0; y<v.size(); y++) {
        for (int x=0; x<v[y].length(); x++) {
            if (( x == 0|| v[y][x-1] > v[y][x]) &&
                (x == v[y].length()-1 || v[y][x+1] > v[y][x]) &&
                (y == 0 || v[y-1][x] > v[y][x]) &&
                (y == v.size()-1 || v[y+1][x] > v[y][x])) 
            {
                total += v[y][x]-'0'+1;
                pq.push({x, y});
            }
        }
    }

    while (!pq.empty()) {
        heap.push_back(DFS(v, visited, pq.front()));
        pq.pop();
    }

    std::make_heap(heap.begin(), heap.end());
    total = heap.front();
    std::pop_heap(heap.begin(), heap.end());
    heap.pop_back();
    total *= heap.front();
    std::pop_heap(heap.begin(), heap.end());
    heap.pop_back();
    total *= heap.front();
    std::cout << total;

    return 0;
}