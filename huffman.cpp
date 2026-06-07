#include "huffman.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* buildTree(int frequency[256]){
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Initially pushing all into minHeap
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            Node* node = new Node;
            node->data = (unsigned char)i;
            node->freq = frequency[i];
            node->left = nullptr;
            node->right = nullptr;
            minHeap.push(node);
        }
    }

    // Making Huffman Tree
    while(minHeap.size()>1){
        Node* a=minHeap.top();
        minHeap.pop();
        Node* b=minHeap.top();
        minHeap.pop();

        Node* c = new Node;
        c->data='$';
        c->freq=a->freq + b->freq ;
        c->left=a;
        c->right=b;

        minHeap.push(c);
    }

    return minHeap.top();
}