#include<iostream>
#include<fstream>
#include"decoder.h"
using namespace std;

void decode(const char* inputFile){
    // Opening File
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error opening file: " << inputFile << endl;
        return;
    }

    // Reading Header For Frq Count
    int freq[256] = {0};
    inFile.read(reinterpret_cast<char*>(freq), 256 * sizeof(int));

    // Reading Padding
    char padding = 0;
    inFile.read(&padding, 1);

    // Huffman Tree for decoding
    Node* root=buildTree(freq);

    //Outfile Name
    string fullPath = string(inputFile);
    // find last slash
    size_t slashPos = fullPath.find_last_of("/\\");
    string folder   = fullPath.substr(0, slashPos + 1);
    string filename = fullPath.substr(slashPos + 1);      // ".txt.huff"
    // remove .huff from filename only
    filename = filename.substr(0, filename.size() - 5);
    // combine
    string outFileName = folder + "Uncomp_" + filename;
    ofstream outFile(outFileName, ios::binary);

    // decoding
    Node* currNode=root;
    unsigned char current, next;
    inFile.read(reinterpret_cast<char*>(&current), 1);
    while(inFile.read(reinterpret_cast<char*>(&next), 1)) {
        for (int i = 7; i >= 0; i--) {
            int bit = (current >> i) & 1;
            if(bit==0) currNode=currNode->left;
            else currNode=currNode->right;

            // Leaf Node
            if(currNode->left==NULL && currNode->right==NULL){
                outFile.write(reinterpret_cast<char*>(&currNode->data), 1);
                currNode=root;
            }
        }
        current=next;
    }

    // Last byte That has Padding
    for (int i = 7; i >= padding; i--) {
        int bit = (current >> i) & 1;
        if(bit==0) currNode=currNode->left;
        else currNode=currNode->right;
        if(currNode->left==nullptr && currNode->right==nullptr){
            outFile.put(currNode->data);
            currNode=root;
        }
    }

    // Closing Files
    inFile.close();
    outFile.close();
}