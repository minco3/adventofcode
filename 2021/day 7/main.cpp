#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

int partition5(std::vector<int>& v, int left, int right) {
    for (int i=left+1; i<right; i++) {
        for (int j=i; j>left && v[j-1] > v[j]; j--) {
            std::swap(v[j-1], v[j]);
        }
    }
    return (left+right)/2;
}

int pivot(std::vector<int>& v, int left, int right) {
    if (right-left<5) {
        return partition5(v, 0, right-left);
    } else {
        for (int i=0; i<v.size(); i+=5) {
            int median5 = partition5(v, i, i+5>v.size()?v.size():i+5);
            std::swap(v[median5], v[left+(i-left)/5]);
        }
    }
    return pivot(v, left, (right+5-1)/5);
}

int main() {
    std::vector<int> v;

    std::fstream file("input.txt");

    while (!file.eof()) {
        int num;
        file >> num;
        v.push_back(num);
        file.ignore(1);
    }
    //pivot(v, 0, v.size());

    unsigned long long sum = 0;
    for (int value : v) {
        sum += value;
    }

    int medianValue = floor(sum/float(v.size()));

    unsigned long long totalfuel = 0;
    for (int value : v) {
        int distance = abs(medianValue-value);
        totalfuel+=distance*(distance+1)/2;
    }
    std::cout << totalfuel;
    return 0;
}