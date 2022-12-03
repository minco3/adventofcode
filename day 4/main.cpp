#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

struct Point {
    int x;
    int y;
    int board;
};

struct Board {
    int values[5][5];
    bool states[5][5] {false};
    int cols[5] {0};
    int rows[5] {0};
    bool hasWon = false;
};

int getScore(Board b, int lastCalled) {
    int total = 0;
    for (int x=0; x<5; x++) {
        for (int y=0; y<5; y++) {
            if (!b.states[x][y]) total += b.values[x][y];
        }
    }
    return total*lastCalled;
}

int main() {
    std::fstream file("test.txt");
    std::string str, a;
    std::vector<Board> boards;
    std::vector<int> values;
    std::unordered_multimap<int, Point> map;
    int num;

    std::getline(file, str);
    std::stringstream ss(str);
    while (!ss.eof()) {
        std::getline(ss, a, ',');
        values.push_back(stoi(a));
    }

    for (int i=0; !file.eof(); i++) {
        std::getline(file, str); // empty line
        boards.emplace_back();
        
        for (int y=0; y<5; y++) {
            std::getline(file, str);
            std::stringstream ss(str);
            for (int x=0; x<5; x++) {
                ss >> num;
                map.emplace(num, Point{x, y, i});
                boards[i].values[x][y] = num;
            }
        }
    }

    int wonCount = 0;

    for (int value : values) {
        auto its = map.equal_range(value);

        for (auto it = its.first; it != its.second; ++it) {
            Point p = it->second;
            boards[p.board].states[p.x][p.y] = true;
            if (++boards[p.board].cols[p.x] == 5 || ++boards[p.board].rows[p.y] == 5) {
                if (!boards[p.board].hasWon) {
                    boards[p.board].hasWon = true;
                    wonCount++;
                    if (wonCount == boards.size()) {
                        std::cout << getScore(boards[p.board], value);
                    }
                }
            }
        }
    }


    return 0;
}