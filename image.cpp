#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image.h"
#include <fstream>
#include <iostream>
using namespace std;

void compressImage(const char* inputFile){
    //load image into pixels
    int width, height, channels;
    unsigned char* pixels = stbi_load(inputFile, &width, &height, &channels, 0);
    
    if(pixels == NULL){
        cout << "Error loading image!" << endl;
        return;
    }

    int totalBytes = width * height * channels;

    //frequency of pixel
    int freq[256] = {0};
    for(int i = 0; i < totalBytes; i++){
        freq[pixels[i]]++;
    }

    //build tree and generate codes
    Node* root = buildTree(freq);
    string codes[256];
    generateCodes(root, "", codes);

    // output file
    string outFile = string(inputFile) + ".huff";
    ofstream out(outFile, ios::binary);

    // write header
    out.write(reinterpret_cast<char*>(&width),    sizeof(int));
    out.write(reinterpret_cast<char*>(&height),   sizeof(int));
    out.write(reinterpret_cast<char*>(&channels), sizeof(int));
    out.write(reinterpret_cast<char*>(freq),      256 * sizeof(int));

    // padding placeholder
    streampos paddingPos = out.tellp();
    char padding = 0;
    out.write(&padding, 1);

    // encode pixels and write bits
    unsigned char buffer = 0;
    int bitCount = 0;

    for(int i = 0; i < totalBytes; i++){
        string code = codes[pixels[i]];
        for(char bit : code){
            if(bit == '1'){
                buffer |= (1 << (7 - bitCount));
            }
            bitCount++;
            if(bitCount == 8){
                out.write(reinterpret_cast<char*>(&buffer), 1);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // write remaining bits
    if(bitCount > 0){
        out.write(reinterpret_cast<char*>(&buffer), 1);
    }

    // fix padding
    padding = (bitCount == 0) ? 0 : (8 - bitCount);
    out.seekp(paddingPos);
    out.write(&padding, 1);

    // cleanup
    out.close();
    stbi_image_free(pixels);
}

void decompressImage(const char* inputFile){

    // Read file and get info
    ifstream inFile(inputFile, ios::binary);

    if(!inFile.is_open()){
        cout << "Error opening file!" << endl;
        return;
    }

    int width, height, channels;
    inFile.read(reinterpret_cast<char*>(&width),    sizeof(int));
    inFile.read(reinterpret_cast<char*>(&height),   sizeof(int));
    inFile.read(reinterpret_cast<char*>(&channels), sizeof(int));

    int freq[256] = {0};
    inFile.read(reinterpret_cast<char*>(freq), 256 * sizeof(int));

    char padding = 0;
    inFile.read(&padding, 1);

    // build tree
    Node* root = buildTree(freq);

    int totalBytes = width * height * channels;
    unsigned char* pixels = new unsigned char[totalBytes];
    int pixelIndex = 0;

    string outFileName = "Uncomp_" + string(inputFile);
    outFileName = outFileName.substr(0, outFileName.size() - 5); // remove .huff

    // decode bits into pixels array
    Node* currNode = root;
    unsigned char current, next;
    inFile.read(reinterpret_cast<char*>(&current), 1);

    while(inFile.read(reinterpret_cast<char*>(&next), 1)){
        for(int i = 7; i >= 0; i--){
            int bit = (current >> i) & 1;
            if(bit == 0) currNode = currNode->left;
            else currNode = currNode->right;

            if(currNode->left == nullptr && currNode->right == nullptr){
                pixels[pixelIndex++] = currNode->data;  // ← into array not file
                currNode = root;
            }
        }
        current = next;
    }

    // last byte with padding
    for(int i = 7; i >= padding; i--){
        int bit = (current >> i) & 1;
        if(bit == 0) currNode = currNode->left;
        else currNode = currNode->right;

        if(currNode->left == nullptr && currNode->right == nullptr){
            pixels[pixelIndex++] = currNode->data;
            currNode = root;
        }
    }
    stbi_write_png(outFileName.c_str(), width, height, channels, pixels, width * channels);

    // clean
    delete[] pixels;
    inFile.close();
}