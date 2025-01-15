#include "encode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decodeFile(const char* inputFilename, const char* outputFilename, HuffmanTreeNode root[], int numNode, int character[]) {
    // 打开输入和输出文件
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    unsigned char bitBuffer; // 用于读取文件中的字节
    int currentNodeIndex = 2 * numNode - 1; // 从根节点开始解码
    int readBits = 0; // 读取的位数
    int bitsBuffer = 0; // 用于存储已读取的比特

    // 逐字节读取输入文件
    while (fread(&bitBuffer, sizeof(unsigned char), 1, inputFile) == 1) {
        for (int i = 0; i < 8; i++) { // 每次处理一个字节的8位
            int bit = (bitBuffer >> (7 - i)) & 1; // 提取当前位

            // 将新读取的位添加到 bitsBuffer
            bitsBuffer = (bitsBuffer << 1) | bit; // 将 bitsBuffer 左移并添加新位
            readBits++; // 更新读取的位数

            // 在哈夫曼树中导航
            currentNodeIndex = 2 * numNode - 1; // 从根节点开始
            currentNodeIndex = root[currentNodeIndex].left;
            for (int j = readBits - 1; j >= 0; j--) {
                int currentBit = (bitsBuffer >> j) & 1; // 获取当前位
                if (currentBit == 0) {
                    currentNodeIndex = root[currentNodeIndex].left; // 向左走
                } else {
                    currentNodeIndex = root[currentNodeIndex].right; // 向右走
                }

                // 检查是否到达叶子节点
                if (root[currentNodeIndex].left == -1 && root[currentNodeIndex].right == -1) {
                    // 找到字符，写入输出文件
                    int ch = character[currentNodeIndex]; // 获取字符
                    bitsBuffer = 0;
                    readBits = 0;
                    // 输出字符（处理中文字符）
                    if (ch < 0x80) { // ASCII 字符
                        fputc(ch, outputFile);
                    } else { // 中文字符，需用两个字节输出
                        fputc((ch >> 8) & 0xFF, outputFile); // 写入高位字节
                        fputc(ch & 0xFF, outputFile); // 写入低位字节
                    }

                    // 找到一个字符后退出循环
                    break;
                }
            }
        }
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}

void decodeFile_Word(const char* inputFilename, const char* outputFilename, HuffmanTreeNode root[], int numNode, int character_Word[]) {
    // 打开输入和输出文件
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    unsigned char bitBuffer; // 用于读取文件中的字节
    int currentNodeIndex = 2 * numNode - 1; // 从根节点开始解码
    //printf("%d",currentNodeIndex);
    int readBits = 0; // 读取的位数
    int bitsBuffer = 0; // 用于存储已读取的比特

    // 逐字节读取输入文件
    while (fread(&bitBuffer, sizeof(unsigned char), 1, inputFile) == 1) {
        for (int i = 0; i < 8; i++) { // 每次处理一个字节的8位
            int bit = (bitBuffer >> (7 - i)) & 1; // 提取当前位

            // 将新读取的位添加到 bitsBuffer
            bitsBuffer = (bitsBuffer << 1) | bit; // 将 bitsBuffer 左移并添加新位
            readBits++; // 更新读取的位数

            // 在哈夫曼树中导航
            currentNodeIndex = 2 * numNode - 1; // 从根节点开始
            currentNodeIndex = root[currentNodeIndex].left;
            //printf("%d",currentNodeIndex);
            for (int j = readBits - 1; j >= 0; j--) {
                int currentBit = (bitsBuffer >> j) & 1; // 获取当前位
                if (currentBit == 0) {
                    currentNodeIndex = root[currentNodeIndex].left; // 向左走
                } else {
                    currentNodeIndex = root[currentNodeIndex].right; // 向右走
                }

                // 检查是否到达叶子节点
                if (root[currentNodeIndex].left == -1 && root[currentNodeIndex].right == -1) {
                    // 找到字符或单词编号
                    int ch = character_Word[currentNodeIndex]; // 获取字符或单词编号
                    bitsBuffer = 0;
                    readBits = 0;

                    // 判断是否为单词编号
                    if (ch >= 0x80 && ch < 0x8000) { 
                        // 处理单词
                        //printf("%d\n",character);
                        for (int k = 0; k < wordCount; k++) {
                            if (sortedWords[k].id == ch) {
                                fputs(sortedWords[k].word, outputFile); // 输出单词
                                break;
                            }
                        }
                    } else if (ch < 0x80) {
                        // 输出 ASCII 字符
                        fputc(ch, outputFile);
                    } else {
                        // 输出中文字符
                        fputc((ch >> 8) & 0xFF, outputFile); // 写入高位字节
                        fputc(ch & 0xFF, outputFile); // 写入低位字节
                    }

                    // 找到一个字符后退出循环
                    break;
                }
            }
        }
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}
