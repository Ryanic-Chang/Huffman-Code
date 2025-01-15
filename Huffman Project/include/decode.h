#ifndef DECODE_H
#define DECODE_H

#include "huffman.h"
#include "word_count.h"
#define BUFFER_SIZE 10240  // 定义缓冲区大小

// 解码函数声明
void decodeFile(const char* inputFilename, const char* outputFilename, HuffmanTreeNode root[], int numNode, int character[]);
void decodeFile_Word(const char* inputFilename, const char* outputFilename, HuffmanTreeNode root[], int numNode, int character_Word[]);

#endif // DECODE_H
