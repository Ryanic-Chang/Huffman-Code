#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 2500         // 最多单词数量
#define MAX_WORD_LENGTH 100    // 单词的最大长度

// 单词频率结构体
typedef struct {
    char word[MAX_WORD_LENGTH]; // 单词
    int count;                  // 单词频率
    int id;                     // 单词编号
} WordFrequency;

extern WordFrequency sortedWords[MAX_WORDS]; // 用于存储排序后的单词
extern int wordCount; // 当前统计的单词数量

// 函数声明
void countWords(const char* inputFilename, const char* outputFilename);
void sortWords(WordFrequency* words, int count);

#endif // WORD_COUNT_H
