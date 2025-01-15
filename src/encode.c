#include "encode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encodeFile(const char* inputFilename, const char* outputFilename, char** code, HuffmanTreeNode root[], int numNode, int character[]) {
    // 打开输入和输出文件
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    int bitBuffer = 0; // 用于存储当前字节的位数据
    int bitCount = 0;  // 当前字节中的有效位数
    char buffer[BUFFER_SIZE];

    // 逐行读取输入文件
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            unsigned char ch = (unsigned char)buffer[i];
            int loc;

            // 处理 ASCII 字符（0到127）
            if (ch < 0x80) {
                // 查找字符在哈夫曼树中的位置
                for (int j = 0; j < numNode; j++) {
                    if (ch == character[j]) {
                        //printf("%d\n",j);
                        loc = j; // 找到该字符对应的编码位置
                        break;
                    }
                }

                // 获取对应的哈夫曼编码
                int len = strlen(code[loc]);
                for (int j = 0; j < len; j++) {
                    // 将编码位添加到 bitBuffer 中
                    bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                    bitCount++;

                    // 如果位数达到8，写入输出文件
                    if (bitCount == 8) {
                        fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                        bitBuffer = 0; // 清空 bitBuffer
                        bitCount = 0;  // 重置位计数
                    }
                }
            }
            // 处理 GBK 中文字符（两个字节）
            else if (i + 1 < strlen(buffer)) {
                unsigned char nextCh = (unsigned char)buffer[i + 1];
                if ((ch >= 0x80 && ch <= 0xFE) && (nextCh >= 0x40 && nextCh <= 0xFE)) {
                    int gbkChar = (ch << 8) | nextCh; // 组合两个字节
                    for (int j = 0; j < numNode; j++) {
                        if (gbkChar == character[j]) {
                            //printf("%d\n",j);
                            loc = j; // 找到该中文字符对应的编码位置
                            break;
                        }
                    }
                    // 获取对应的哈夫曼编码
                    int len = strlen(code[loc]);
                    for (int j = 0; j < len; j++) {
                        // 将编码位添加到 bitBuffer 中
                        bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                        bitCount++;

                        // 如果位数达到8，写入输出文件
                        if (bitCount == 8) {
                            fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                            bitBuffer = 0; // 清空 bitBuffer
                            bitCount = 0;  // 重置位计数
                        }
                    }
                    i++; // 跳过下一个字节
                }
            }
        }
    }

    // 如果还有剩余的位没有写入，则补齐并写入输出文件
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount); // 左移补齐
        fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}

void encodeFile_Word(const char* inputFilename, const char* outputFilename, char** code, HuffmanTreeNode root[], int numNode, int character_Word[]) {
    // 打开输入和输出文件
    FILE *inputFile = fopen(inputFilename, "rb");
    FILE *outputFile = fopen(outputFilename, "wb");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return;
    }

    int bitBuffer = 0; // 用于存储当前字节的位数据
    int bitCount = 0;  // 当前字节中的有效位数
    char buffer[BUFFER_SIZE];

    // 逐行读取输入文件
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        int i = 0;
        while (buffer[i] != '\0') {
            if ((buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z')) {
                // 处理单词
                char word[MAX_WORD_LENGTH] = {0};
                int wordIndex = 0;

                // 提取单词
                while ((buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z')) {
                    word[wordIndex++] = buffer[i++];
                }
                word[wordIndex] = '\0';

                // 如果单词仅有一个字符，按单字符逻辑处理
                if (strlen(word) == 1) {
                    unsigned char ch = (unsigned char)word[0];
                    int loc;
                    for (int j = 0; j < numNode; j++) {
                        if (ch == character_Word[j]) {
                            loc = j;
                            break;
                        }
                    }

                    int len = strlen(code[loc]);
                    for (int j = 0; j < len; j++) {
                        bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                        bitCount++;
                        if (bitCount == 8) {
                            fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                            bitBuffer = 0;
                            bitCount = 0;
                        }
                    }
                } else {
                    // 处理两个及以上字符的单词
                    for (int k = 0; k < wordCount; k++) {
                        if (strcmp(word, sortedWords[k].word) == 0) {
                            int id = sortedWords[k].id;
                            int loc;
                            for (int j = 0; j < numNode; j++) {
                                if (id == character_Word[j]) {
                                    loc = j;
                                    break;
                                }
                            }

                            int len = strlen(code[loc]);
                            for (int j = 0; j < len; j++) {
                                bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                                bitCount++;
                                if (bitCount == 8) {
                                    fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                                    bitBuffer = 0;
                                    bitCount = 0;
                                }
                            }
                            break;
                        }
                    }
                }
            } else if ((unsigned char)buffer[i] < 0x80) {
                // 处理 ASCII 字符（与 encodeFile 保持一致）
                unsigned char ch = (unsigned char)buffer[i];
                int loc;
                for (int j = 0; j < numNode; j++) {
                    if (ch == character_Word[j]) {
                        loc = j;
                        break;
                    }
                }

                int len = strlen(code[loc]);
                for (int j = 0; j < len; j++) {
                    bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                    bitCount++;
                    if (bitCount == 8) {
                        fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                        bitBuffer = 0;
                        bitCount = 0;
                    }
                }
                i++;
            } else if ((unsigned char)buffer[i] >= 0x80) {
                // 处理 GBK 中文字符（与 encodeFile 保持一致）
                unsigned char ch1 = (unsigned char)buffer[i];
                unsigned char ch2 = (unsigned char)buffer[i + 1];
                if ((ch1 >= 0x80 && ch1 <= 0xFE) && (ch2 >= 0x40 && ch2 <= 0xFE)) {
                    int gbkChar = (ch1 << 8) | ch2;
                    int loc;
                    for (int j = 0; j < numNode; j++) {
                        if (gbkChar == character_Word[j]) {
                            loc = j;
                            break;
                        }
                    }

                    int len = strlen(code[loc]);
                    for (int j = 0; j < len; j++) {
                        bitBuffer = (bitBuffer << 1) | (code[loc][j] - '0');
                        bitCount++;
                        if (bitCount == 8) {
                            fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                            bitBuffer = 0;
                            bitCount = 0;
                        }
                    }
                    i += 2;
                }
            } else {
                i++; // 跳过其他字符
            }
        }
    }

    // 如果还有剩余的位没有写入，则补齐并写入输出文件
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}
