#include <fstream>
#include <iostream>
#include <queue>

static const int days = 256;

int main() {

    unsigned long long ages[9] {0};

    std::fstream file("test.txt");
    unsigned long long count = 0;
    int day = 0;

    while (!file.eof()) {
        int num;
        file >> num;
        ages[num]++;
        count++;
        file.ignore(1);
    }

    for (int i=0; i<days; i++, day++) {
        if (day > 8) day = 0;

        count += ages[day];

        ages[(day+7)%9] += ages[day];

    }
    
    std::cout << count;
    return 0;
}