#include <iostream>
#include <fstream>
#include <iomanip>

int main() { // static method
    std::fstream file("input.txt");
    int count = 0, arr[4], current_no = 0, size = 0;

    while(!file.eof()) {
        file >> arr[current_no++];
        if (++size < 4) continue;
        if (current_no == 4) current_no = 0;
        if (arr[(current_no+3)%4] > arr[current_no]) count++;
    }

    std::cout << count;
}

// #include <queue>

// int main() { // queue method
//     std::fstream file("input.txt");
//     int count = 0, val;
//     std::queue<int> v;
//     while(!file.eof()) {
//         file >> val;
//         v.push(val);
//         if (v.size() < 4) continue;
//         if (v.front() < v.back()) count++;
//         v.pop();
//     }
//     std::cout << count;
// }

/*

199  A      
200  A B    
208  A B C  
210    B C D
200  E   C D
207  E F   D
240  E F G  
269    F G H
260      G H
263        H

[A, B, C, D]
[E, B, C, D]
[E, F, C, D]
[E, F, G, H]

Queue


*/
