
/*
huffman_compressor/
├── main.cpp
├── frequency.h
├── frequency.cpp
├── huffman.h
├── huffman.cpp
├── encoder.h
├── encoder.cpp
├── decoder.h
└── decoder.cpp   */

#include <iostream>
#include "frequency.h"
#include "huffman.h"
#include "encoder.h"
#include "decoder.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./compressor <filename>" << endl;
        return 1;
    }

    // Frequency 
    int frequency[256] = {0};
    countFrequency(argv[1], frequency);

    // builf huffman tree
    Node* root = buildTree(frequency);

    // Encoder
    string codes[256];
    generateCodes(root,"",codes);

    string inputFile = string(argv[1]);
    string outputFile = string(argv[1]) + ".huff";
    encode(inputFile.c_str(),outputFile.c_str(),codes,frequency);

    // Decoder 
    decode(outputFile.c_str());
    return 0;
}