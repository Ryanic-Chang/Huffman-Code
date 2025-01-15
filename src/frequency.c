#include "frequency.h"
#include "linked_list.h"
#include <stdio.h>
#include <string.h>

void calculateFrequency(const char* filename, ListNode** charList) {
    FILE *file = fopen(filename, "rb");  // 以二进制模式打开文件
    if (!file) {
        perror("Unable to open file");
        return;
    }

    char buffer[BUFFER_SIZE];  // 用于存储读取的行
    int totalChars = 0;

    // 逐行读取文件内容
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            unsigned char ch = (unsigned char)buffer[i];

            // 处理 ASCII 字符（0到127）
            if (ch < 0x80) {
                addOrUpdateNode(charList, ch);  // 直接传入 charList
                totalChars++;
            } 
            // 处理 GBK 中文字符（两个字节）
            else if (i + 1 < strlen(buffer)) {
                unsigned char nextCh = (unsigned char)buffer[i + 1];
                if ((ch >= 0x80 && ch <= 0xFE) && (nextCh >= 0x40 && nextCh <= 0xFE)) {
                    int gbkChar = (ch << 8) | nextCh;  // 将两个字节组合
                    addOrUpdateNode(charList, gbkChar);  // 直接传入 charList
                    totalChars++;
                    i++;  // 跳过下一个字节
                }
            }
        }
    }

    fclose(file);

    FILE *statFile = fopen("output/Statistic.txt", "w+");
    if (!statFile) {
        perror("Unable to open statistic file");
        return;
    }

    ListNode* current = *charList;  // 修正为解引用 charList
    current = sortList(current);
    *charList = current;
    while (current != NULL) {
        double frequency = (double)current->frequency / totalChars;

        if (current->character < 0x80) {
            if (current->character == '\n') {
                fprintf(statFile, "Character: '\\n' Frequency: %d (%.4f)\n", current->frequency, frequency);
            } else if (current->character == '\t') {
                fprintf(statFile, "Character: '\\t' Frequency: %d (%.4f)\n", current->frequency, frequency);
            } else if (current->character == '\r') {
                fprintf(statFile, "Character: '\\r' Frequency: %d (%.4f)\n", current->frequency, frequency);
            } else {
                fprintf(statFile, "Character: '%c' Frequency: %d (%.4f)\n", current->character, current->frequency, frequency);
            }
        } else { 
            unsigned char firstByte = (current->character >> 8) & 0xFF;
            unsigned char secondByte = current->character & 0xFF;
            fprintf(statFile, "Character: '%c%c' Frequency: %d (%.4f)\n", firstByte, secondByte, current->frequency, frequency);
        }

        current = current->next;
    }

    fclose(statFile);
}
