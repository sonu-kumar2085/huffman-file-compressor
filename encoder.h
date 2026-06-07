#ifndef ENCODER_H
#define ENCODER_H

#include "huffman.h"
#include <string>

using namespace std;

void generateCodes(Node* root, string code, string codes[256]);
void encode(const char* inputFile, const char* outputFile, string codes[256],int frequency[256]);

#endif