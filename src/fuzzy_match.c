#include "fuzzy_match.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 计算 Levenshtein 距离
int levenshteinDistance(const char* s1, const char* s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    // 创建动态数组以存储距离
    int** dp = (int**)malloc((len1 + 1) * sizeof(int*));
    for (int i = 0; i <= len1; i++) {
        dp[i] = (int*)malloc((len2 + 1) * sizeof(int));
    }

    // 初始化
    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i; // 删除操作
    }
    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j; // 插入操作
    }

    // 填充动态数组
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1; // 匹配成本
            dp[i][j] = fmin(fmin(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost); // 取最小值
        }
    }

    int distance = dp[len1][len2]; // 获取最后的距离

    // 释放动态数组
    for (int i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);

    return distance;
}

// 模糊匹配文件中的字符串
void fuzzyMatchInFile(const char* pattern) {
    FILE *file = fopen("data/SourceDoc.txt", "r");
    if (!file) {
        perror("Unable to open source document");
        return;
    }

    char buffer[10240];
    char paragraph[20480] = ""; // 用于存储当前段落的内容
    int paragraphIndex = 1;
    int foundMatch = 0; // 记录是否找到匹配

    printf("You are looking for the string: \"%s\"\n", pattern);
    printf("Search results:\n");

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        strcat(paragraph, buffer); // 将当前行添加到段落中

        char* token = strtok(buffer, " \n"); // 分隔字符串以处理单词
        char* lastMatch = NULL; // 记录最后匹配的单词

        while (token) {
            int distance = levenshteinDistance(token, pattern);
            if (distance <= 2) { // 设置阈值，允许2个字符的差异
                lastMatch = token; // 记录当前找到的匹配
                break; // 如果找到匹配，可以退出内部循环
            }
            token = strtok(NULL, " \n");
        }

        if (lastMatch) {
            // 输出整段并强调匹配的单词
            printf("Found in paragraph %d: \"%s\"\n", paragraphIndex, lastMatch); // 输出找到的模糊匹配单词
            printf("%s\n", paragraph); // 输出整段
            foundMatch = 1; // 标记为找到匹配
        }

        // 段落结束时处理
        if (buffer[strlen(buffer) - 1] == '\n' && strlen(buffer) == 1) { 
            // 仅为换行的情况下
            paragraphIndex++;
            memset(paragraph, 0, sizeof(paragraph)); // 清空段落内容
        }
    }

    // 提示搜索完成
    if (!foundMatch) {
        printf("No matches found for the string \"%s\".\n", pattern);
    }
    printf("Search completed for the string \"%s\".\n", pattern); // 在任何情况下都提示搜索完成

    fclose(file);
}
