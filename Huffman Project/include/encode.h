// encode.h
#ifndef ENCODE_H
#define ENCODE_H

#include "huffman.h"
#define BUFFER_SIZE 10240 // 缓冲区大小

void encodeFile(const char* inputFilename, const char* outputFilename, char** codes,HuffmanTreeNode root[],int numNode, int character[]);
void encodeFile_Word(const char* inputFilename, const char* outputFilename, char** codes,HuffmanTreeNode root[],int numNode, int character_Word[]);

#endif // ENCODE_H
