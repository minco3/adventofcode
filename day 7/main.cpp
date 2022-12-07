#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

struct node {
    int weight;
    unordered_map<string, node*> subdirs;
    node() : weight(0), subdirs() {}
};

int recursiveadd(node* n) {
    int sum = 0;
    if (n->weight < 100000) {
        sum = n->weight;
    }
    for (auto p : n->subdirs) {
        sum += recursiveadd(p.second);
    }
    return sum;
}

void findvalid(vector<int>& v, node* n, int neededspace) {
    if (n->weight >= neededspace) {
        v.push_back(n->weight);
        for (auto p : n->subdirs) {
            findvalid(v, p.second, neededspace);
        }
    }
}

int main() {
    // fstream file("test.txt");
    fstream file("input.txt");
    deque<int> q;
    deque<char> dq;
    unordered_map<int, int> map;
    node rootdir;
    vector<int> sizes;
    stack<node*> s;
    stack<int> is;
    string str, str2, ans;
    vector<int> v;
    int sum = 0, var2 = 0, level = 0, smallestdir, usedspace = 0;

    node* currentdir = &rootdir; 
    
    getline(file, str); //cd /
    getline(file, str); //ls

    while (file.peek() != '$') { // root dir
        getline(file, str);
        if (str[0] != '$' && str[0] == 'd') { // dir
            rootdir.subdirs.insert({str.substr(4), new node()});
        } else if (str[0] != 'd') {
            stringstream ss(str);
            ss >> str2;
            usedspace += stoi(str2);
        }
    }

    while (!file.eof()) {
        getline(file, str);
        if (str[0] == '$' && str[2] == 'c' && str[5] != '.')  { // level in
            level++;
            s.push(currentdir);
            currentdir = currentdir->subdirs.at(str.substr(5));
        } else if (str[0] == '$' && str[2] == 'c' && str[5] == '.') { // level out
            level--;
            is.push(currentdir->weight);
            currentdir = s.top();
            s.pop();
            currentdir->weight+= is.top();
            is.pop();
        } else if (str[0] == '$' && str[2] == 'l') { // ls
            //ignore
        } else if (isdigit(str[0])) { // file
            stringstream ss(str);
            ss >> str2;
            currentdir->weight+=stoi(str2);
            usedspace += stoi(str2);
        } else { // dir
            currentdir->subdirs.insert({str.substr(4), new node()});
        }
    }

    int neededspace = usedspace - 40000000;

    for (auto p : rootdir.subdirs) {
        for (auto p2 : p.second->subdirs) {
            sum += recursiveadd(p2.second);
        }
        if (p.second->weight < 100000) {
            sum += p.second->weight;
        }
        findvalid(v, p.second, neededspace);
    }

    smallestdir = *min_element(v.begin(), v.end());

    std::cout << sum << " " << smallestdir;
}