
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
#include "image.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./compressor <filename>" << endl;
        return 1;
    }

    // file detection
    string filename = string(argv[1]);
    size_t dotPos = filename.find_last_of('.');
    string ext = filename.substr(dotPos);

    if(ext == ".txt"){
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
    }
    else if(ext == ".jpg" || ext == ".png"){
        // image compression
        compressImage(argv[1]);
    }
    else if(ext == ".huff"){
        // decompress
        // part before .huff
        string nameWithoutHuff = filename.substr(0, filename.size() - 5);
    
        // extension before .huff
        size_t dotPos2 = nameWithoutHuff.find_last_of('.');
        string originalExt = nameWithoutHuff.substr(dotPos2);

        if(originalExt == ".txt"){
            decode(argv[1]);
        }
        else if(originalExt == ".jpg" || originalExt == ".png"){
            decompressImage(argv[1]);
        }
        else{
            cout << "Unknown format inside .huff!" << endl;
        }
    }
    else{
        cout << "Unsupported file format!" << endl;
    }

    return 0;
}