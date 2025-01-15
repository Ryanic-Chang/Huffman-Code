#include "word_count.h"
#include "linked_list.h"

#define MAX_WORDS 2500
#define MAX_WORD_LENGTH 100

WordFrequency sortedWords[MAX_WORDS]; // 用于存储排序后的单词
int wordCount; // 当前统计的单词数量

// 按字母顺序排序单词
void sortWords(WordFrequency* words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(words[i].word, words[j].word) > 0) {
                WordFrequency temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

void countWords(const char* inputFilename, const char* outputFilename) {
    FILE* file = fopen(inputFilename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char buffer[10240]; // 临时存储每行的内容
    wordCount = 0;      // 统计单词总数

    // 逐行读取文件内容
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int start = -1; // 记录当前单词的起始位置

        for (int i = 0; buffer[i] != '\0'; i++) {
            char ch = buffer[i];

            // 检查是否为字母（大写或小写）
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                if (start == -1) {
                    start = i; // 标记单词开始
                }
            } else {
                // 如果遇到非字母字符且之前记录了单词起始位置
                if (start != -1) {
                    char token[MAX_WORD_LENGTH] = {0};
                    int len = i - start;

                    // 提取单词并以 '\0' 结束
                    strncpy(token, buffer + start, len);
                    token[len] = '\0';

                    // 插入单词到统计数组
                    int found = 0;
                    for (int j = 0; j < wordCount; j++) {
                        if (strcmp(sortedWords[j].word, token) == 0) {
                            sortedWords[j].count++; // 增加频率
                            found = 1;
                            break;
                        }
                    }
                    if (!found && wordCount < MAX_WORDS) {
                        strcpy(sortedWords[wordCount].word, token);
                        sortedWords[wordCount].count = 1;
                        sortedWords[wordCount].id = 0x80 + wordCount; // 编号从0x80开始
                        wordCount++;
                    }

                    start = -1; // 重置单词起始位置
                }
            }
        }

        // 处理行末最后一个单词
        if (start != -1) {
            char token[MAX_WORD_LENGTH] = {0};
            int len = strlen(buffer) - start;

            strncpy(token, buffer + start, len);
            token[len] = '\0';

            int found = 0;
            for (int j = 0; j < wordCount; j++) {
                if (strcmp(sortedWords[j].word, token) == 0) {
                    sortedWords[j].count++;
                    found = 1;
                    break;
                }
            }
            if (!found && wordCount < MAX_WORDS) {
                strcpy(sortedWords[wordCount].word, token);
                sortedWords[wordCount].count = 1;
                sortedWords[wordCount].id = 0x80 + wordCount;
                wordCount++;
            }
        }
    }

    fclose(file);

    // 按字母顺序排序单词
    sortWords(sortedWords, wordCount);

    // 将排序后的单词及其频率与编号写入文件
    FILE* output = fopen(outputFilename, "w");
    if (!output) {
        perror("Error opening file for writing");
        return;
    }

    fprintf(output, "Word, Frequency, ID\n");
    for (int i = 0; i < wordCount; i++) {
        fprintf(output, "%s, %d, %d\n", sortedWords[i].word, sortedWords[i].count, sortedWords[i].id);
    }
    fclose(output);
}
