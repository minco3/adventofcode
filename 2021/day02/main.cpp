#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>

// int main() {
//     std::fstream file("input.txt");
//     int count = 0, arr[4], current_no = 0;
//     for (int i=0; i<4; i++) {
//         file >> arr[i]; 
//     }
//     while(!file.eof()) {
//         if (current_no>3) current_no = 0;
//         if (arr[(current_no+3)%4] > arr[current_no])
//             count++;
//         file >> arr[current_no++];
//     }
//     if (arr[(current_no+3)%4] > arr[current_no])
//         count++;
//     std::cout << count;
// }

int main() {
    std::fstream file("test.txt");
    int depth = 0, pos = 0, aim = 0;
    char buf[15];
    while (!file.eof()) {
        file.getline(buf, 15);

        switch (buf[0]) {
        case 'f': //forward
            depth += (buf[8]-'0')*aim;
            pos += (buf[8]-'0');
            break;
        case 'd': //down
            aim += buf[5]-'0';
            break;
        case 'u': //up
            aim -= buf[3]-'0';
            break;
        default:
            std::cout << "default";
            break;
        }
    }
    std::cout << depth << " " << pos << '\n';
    std::cout << depth*pos;
}