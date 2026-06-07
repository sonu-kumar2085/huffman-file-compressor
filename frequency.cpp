#include "frequency.h"
#include <fstream>
#include <iostream>
using namespace std;

void countFrequency(const char* filename, int frequency[256]) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        frequency[byte]++;
    }

    file.close();
}