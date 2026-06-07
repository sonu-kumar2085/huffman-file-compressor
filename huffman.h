#ifndef HUFFMAN_H
#define HUFFMAN_H


struct Node {
    unsigned char data;
    int freq;
    Node* left;
    Node* right;
};

Node* buildTree(int frequency[256]);

#endif