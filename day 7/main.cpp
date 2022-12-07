#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

struct node {
    int size;
    map<string, node*> subdirs;
    map<string, int> files;
    node() : size(0), subdirs(), files() {}
};

int recursiveadd(node* n) {
    int sum = n->size < 100000 ? n->size : 0;

    for (const auto& p : n->subdirs) {
        sum += recursiveadd(p.second);
    }
    return sum;
}

int findvalid(int smallestdir, node* n, int neededspace) {
    if (n->size >= neededspace) {
        for (const auto& p : n->subdirs) {
            findvalid(smallestdir, p.second, neededspace);
        }
        if (n->size < smallestdir) {
            return n->size;
        }
    }
    return smallestdir;
}

void cleanup(node* n) {
    for (const auto& p : n->subdirs) {
        cleanup(p.second);
    }
    delete n;
}

void print(node* n, int level = 0) {
    if (level == 0) {
        std::cout << "- / (dir)\n";
    }
    auto files = n->files.begin();
    auto subdirs = n->subdirs.begin();

    while (files != n->files.end() || subdirs!=n->subdirs.end()) {
        if ((files!=n->files.end() && subdirs!=n->subdirs.end() && files->first < subdirs->first) || subdirs==n->subdirs.end() && files != n->files.end()) {
            cout << setw((level+2)*2) << "- " << files->first << " (file, size=" << files->second << ")\n";
            files++;
        } else if (subdirs!=n->subdirs.end()) {
            cout << setw((level+2)*2) << "- " << subdirs->first << " (dir)\n";
            print(subdirs->second, level+1);
            subdirs++;
        }
    }

}

int main() {
    fstream file("test.txt");
    string str;
    int sum = 0;

    node rootdir;
    node* currentdir = &rootdir; 
    stack<node*> s;
    stack<int> weights;
    
    getline(file, str); //cd /

    while (!file.eof()) { // parse
        getline(file, str);
        if (isdigit(str[0])) { // file
            int size;
            string filename;
            stringstream ss(str);
            ss >> size >> filename;
            currentdir->size += size;
            currentdir->files.insert({filename, size});
        } else if (str.substr(0, 4) == "$ cd" && str[5] != '.')  { // cd in
            s.push(currentdir);
            currentdir = currentdir->subdirs.at(str.substr(5));
        } else if (str.substr(0, 4) == "$ cd" && str[5] == '.') { // cd out
            weights.push(currentdir->size); 
            currentdir = s.top(); // this is not assembly
            s.pop();
            currentdir->size += weights.top();
            weights.pop();
        } else if (str.substr(0, 3) == "dir") { // dir
            currentdir->subdirs.insert({str.substr(4), new node()});
        }
    }

    rootdir.size += currentdir->size;

    int smallestdir = rootdir.size, neededspace = rootdir.size - 40000000;

    print(&rootdir); // why not

    for (const auto& p : rootdir.subdirs) { // solve
        sum += recursiveadd(p.second);
        smallestdir = findvalid(smallestdir, p.second, neededspace);
        cleanup(p.second);
    }

    std::cout << sum << " " << smallestdir;
}