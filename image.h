#ifndef IMAGE_H
#define IMAGE_H

#include "huffman.h"
#include "encoder.h"
#include "frequency.h"

void compressImage(const char* inputFile);
void decompressImage(const char* inputFile);

#endif