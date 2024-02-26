#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <bitset>


int main() {

    std::fstream file("input.txt");
    std::vector<std::bitset<7>> bytes;
    std::string str;

    std::bitset<7> byte;

    bytes.resize(10);

    unsigned int total = 0, num;

    while (!file.eof()) {

        std::multimap<int, std::bitset<7>> map;
        for (int i=0; i<10; i++) {
            byte.reset();
            file >> str;
            for (int j=0; j<str.length(); j++) {
                byte.flip(str[j]-'a');
            }
            map.insert({byte.count(), byte});
        }

        //insert values we are able to assume based on # of bits flipped alone
        bytes[1] = map.extract(2).mapped();
        bytes[7] = map.extract(3).mapped();
        bytes[4] = map.extract(4).mapped();
        bytes[8] = map.extract(7).mapped();

        auto its = map.equal_range(5); // 2 3 5
        for (auto it = its.first; it!=its.second; it++){
            if ((it->second & bytes[7]).count() == 3) {
                bytes[3] = it->second;
            } else if ((it->second & bytes[4]).count() == 2) {
                bytes[2] = it->second;
            } else {
                bytes[5] = it->second;
            }
        }

        its = map.equal_range(6); // 6 9 0
        for (auto it = its.first; it != its.second; it++) {
            if ((it->second & bytes[7]).count() == 2) {
                bytes[6] = it->second;
            } else if ((it->second & bytes[4]).count() == 4) {
                bytes[9] = it->second;
            } else {
                bytes[0] = it->second;
            }
        }

        file.ignore(3);
        num = 0;
        for (int i=0; i<4; i++) {
            num*=10;
            byte.reset();
            file >> str;
            for (char c : str) {
                byte.flip(c-'a');
            }
            switch (byte.count()) {
            case 2:
                num += 1;
                break;
            case 3:
                num += 7;
                break;
            case 4:
                num += 4;
                break;
            case 7:
                num += 8;
                break; 
            case 5: // 2 3 5
                if (bytes[2] == byte) {
                    num += 2;
                } else if (bytes[3] == byte) {
                    num += 3;
                } else num += 5;
            break;
            case 6: // 6 9 0
                if (bytes[6] ==  byte) {
                    num += 6;
                } else if (bytes[9] == byte) {
                    num += 9;
                } else num += 0;
            break;
            }
        }
        total += num;
    }


    std::cout << total;

    return 0;
}