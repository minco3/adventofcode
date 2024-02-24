#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>

struct node {

    node(bool _big = false, std::string _name = "") : big(_big), name(_name) {}
    bool big = false;
    std::vector<node*> connections;
    std::string name;
};

int visit (const node& entry, std::unordered_multiset<std::string> visited, bool visitedTwice = false) {
    if (entry.name == "end") return 1;
    int paths = 0;
    if (!entry.big && entry.name != "start") {
        visited.insert(entry.name);
    }
    for (node* connection : entry.connections) {
        if (connection->name == "start") continue;
        if (connection->big) {
            paths += visit(*connection, visited, visitedTwice);
        } else if (!visitedTwice && !visited.count(connection->name)) {
            paths += visit(*connection, visited, visitedTwice);
        } else if (!visitedTwice && visited.count(connection->name) < 2) {
            paths += visit(*connection, visited, true);
        } else if (!visited.count(connection->name)) {
            paths += visit(*connection, visited, visitedTwice);
        }
    }
    return paths;
}

int main() {

    std::fstream file("input.txt");
    std::unordered_map<std::string, node> map;
    std::unordered_multiset<std::string> set;
    std::string str;

    unsigned long long paths = 0;

    while (!file.eof()) {
        std::getline(file, str, '-');
        std::pair<std::unordered_map<std::string, node>::iterator, bool> p, p2;
        node* lhs = &map.insert({str, node(isupper(str[0]), str)}).first->second;
        std::getline(file, str);
        node* rhs = &map.insert({str, node(isupper(str[0]), str)}).first->second;
        lhs->connections.push_back(rhs);
        rhs->connections.push_back(lhs);
    }


    paths = visit(map.find("start")->second, set);
    
    std::cout << paths;

    return 0;
}