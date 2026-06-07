#include "encoder.h"
#include "fstream"
#include <iostream>

using namespace std;

void generateCodes(Node* root, string code, string codes[256]){
    if(root==nullptr) return;

    if(root->left==NULL && root->right==NULL){
        codes[root->data]=code;
        return;
    }

    generateCodes(root->left,code+'0',codes);
    generateCodes(root->right,code+'1',codes);

}

void encode(const char* inputFile, const char* outputFile,string codes[256], int freq[256]) {
    // open files
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    // write header - freq table
    outFile.write(reinterpret_cast<char*>(freq), 256 * sizeof(int));

    // write padding placeholder
    streampos paddingPos = outFile.tellp();
    char padding = 0;
    outFile.write(&padding, 1);

    // bit packing
    unsigned char buffer = 0;
    int bitCount = 0;

    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        string code = codes[byte];
        for (char bit : code) {
            if (bit == '1') {
                buffer |= (1 << (7 - bitCount));
            }
            bitCount++;
            if (bitCount == 8) {
                outFile.write(reinterpret_cast<char*>(&buffer), 1);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // write remaining bits
    if (bitCount > 0) {
        outFile.write(reinterpret_cast<char*>(&buffer), 1);
    }

    // go back write real padding
    padding = (bitCount == 0) ? 0 : (8 - bitCount);
    outFile.seekp(paddingPos);
    outFile.write(&padding, 1);

    inFile.close();
    outFile.close();
}