#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "linked_list.h"

typedef struct HuffmanTreeNode {
    int character; // 使用 int 类型以支持 GBK 字符
    int frequency;
    int parent,left,right;
} HuffmanTreeNode;

void initHuffmanTree(HuffmanTreeNode* root,ListNode* charList,int numNode);
void createHuffmanTree(HuffmanTreeNode* root,int numNode);
void createHuffmanCode(HuffmanTreeNode root[],char*** code,int numNode);
void storeHuffmanCode(const char* filename, char** code, HuffmanTreeNode root[], int numNode);

#endif // HUFFMAN_H
