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
    node* parentdir;
    map<string, node*> subdirs;
    map<string, int> files;
    node(node* _parentnode = nullptr) : size(0), subdirs(), files(), parentdir(_parentnode) {}
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
            smallestdir = findvalid(smallestdir, p.second, neededspace);
        }
        if (n->size < smallestdir) {
            return n->size;
        }
    }
    return smallestdir;
}

void addtoparent(node* n, int val) {
    if (n->parentdir) {
        addtoparent(n->parentdir, val);
    }
    n->size += val;
}

void cleanup(node* n, bool root = true) {
    for (const auto& p : n->subdirs) {
        cleanup(p.second, false);
    }
    if (!root) {
        delete n;
    }
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
    
    getline(file, str); //cd /

    while (!file.eof()) { // parse
        getline(file, str);
        if (isdigit(str[0])) { // file
            int size;
            string filename;
            stringstream ss(str);
            ss >> size >> filename;
            addtoparent(currentdir, size);
            currentdir->files.insert({filename, size});
        } else if (str.substr(0, 4) == "$ cd" && str[5] != '.')  { // cd in
            currentdir = currentdir->subdirs.at(str.substr(5));
        } else if (str.substr(0, 4) == "$ cd" && str[5] == '.') { // cd out
            currentdir = currentdir->parentdir;
        } else if (str.substr(0, 3) == "dir") { // dir
            currentdir->subdirs.insert({str.substr(4), new node(currentdir)});
        }
    }

    //95437 24933642

    int smallestdir = rootdir.size, neededspace = rootdir.size - 40000000;

    print(&rootdir); // why not

    sum = recursiveadd(&rootdir);
    smallestdir = findvalid(smallestdir, &rootdir, neededspace);
    cleanup(&rootdir);

    std::cout << sum << " " << smallestdir;
}