#include "read.h"
#include "word_count.h"
#include <string.h>
#include <stdio.h>

void readFile(const char *filename, int character[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    int count = 0; // 当前已存储的字符数量
    char temp_character[32];
    char line[256]; // 每行的最大长度

    while (fgets(line, sizeof(line), file) && count < MAX_CHAR_COUNT) {
        char *characterPtr = strstr(line, "Character: ");
        char *codePtr = strstr(line, "Code: ");
        if (characterPtr && codePtr) {
            // 提取字符
            characterPtr += strlen("Character: ");
            characterPtr += 1;
            char *charEnd = strchr(characterPtr, ' ');
            charEnd -= 1;
            if (charEnd) *charEnd = '\0';

            // 存储数据到 temp_character
            strncpy(temp_character, characterPtr, sizeof(temp_character) - 1);
            temp_character[sizeof(temp_character) - 1] = '\0'; // 确保以 '\0' 结尾

            // 对特殊字符进行转换
            if (strcmp(temp_character, "\\n") == 0) {
                character[count] = '\n';  // 转换为 ASCII 值 10
            } else if (strcmp(temp_character, "\\r") == 0) {
                character[count] = '\r';  // 转换为 ASCII 值 13
            } else if (strcmp(temp_character, "\\t") == 0) {
                character[count] = '\t';  // 转换为 ASCII 值 9
            } else {
	            // 对 ASCII 和汉字字符分别处理
	            unsigned char ch = (unsigned char)temp_character[0]; // 第一个字符
	            if (ch < 0x80) {
	                // ASCII 字符处理
	                int asciiVal = (int)ch;
	                character[count] = asciiVal; // 存储 ASCII 值
	            } else if (strlen(temp_character) > 1) {
	                // 可能是 GBK 或其他双字节汉字
	                unsigned char nextCh = (unsigned char)temp_character[1];
	                if ((ch >= 0x80 && ch <= 0xFE) && (nextCh >= 0x40 && nextCh <= 0xFE)) {
	                    int gbkChar = (ch << 8) | nextCh; // 将两个字节组合为一个整型值
	                    character[count] = gbkChar;      // 存储 GBK 编码值
	                } else {
	                    printf("无效的字符: %s\n", temp_character);
	                    continue; // 跳过无效字符
	                }
	            } else {
	                printf("无效的字符: %s\n", temp_character);
	                continue; // 跳过无效字符
	            }
            }
            count++;
        }
    }

    fclose(file);
}

void readFile_Word(const char *filename, int character_Word[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return;
    }

    int count = 0; // 当前已存储的字符数量
    char temp_character[32];
    char line[256]; // 每行的最大长度

    while (fgets(line, sizeof(line), file) && count < MAX_CHAR_COUNT) {
        char *characterPtr = strstr(line, "Character: ");
        char *codePtr = strstr(line, "Code: ");
        if (characterPtr && codePtr) {
            // 提取字符部分
            characterPtr += strlen("Character: ");
            characterPtr += 1; // 跳过引号
            char *charEnd = strchr(characterPtr, ' '); // 查找空格
            charEnd -= 1; // 移到字符的最后位置
            if (charEnd) *charEnd = '\0'; // 结束字符

            // 存储数据到 temp_character
            strncpy(temp_character, characterPtr, sizeof(temp_character) - 1);
            temp_character[sizeof(temp_character) - 1] = '\0'; // 确保以 '\0' 结尾

            // 处理特殊字符
            if (strcmp(temp_character, "\\n") == 0) {
                character_Word[count] = '\n';  // 转换为 ASCII 值 10
            } else if (strcmp(temp_character, "\\r") == 0) {
                character_Word[count] = '\r';  // 转换为 ASCII 值 13
            } else if (strcmp(temp_character, "\\t") == 0) {
                character_Word[count] = '\t';  // 转换为 ASCII 值 9
            } else if ((temp_character[0] >= 'a' && temp_character[0] <= 'z') || (temp_character[0] >= 'A' && temp_character[0] <= 'Z')) {
                // 处理单词
                char word[32] = {0};
                int wordIndex = 0;

				while ((temp_character[wordIndex] >= 'a' && temp_character[wordIndex] <= 'z') || 
				       (temp_character[wordIndex] >= 'A' && temp_character[wordIndex] <= 'Z')) {
				    word[wordIndex] = temp_character[wordIndex];
				    wordIndex++;
				}

                word[wordIndex] = '\0'; // 结束单词

                // 如果是单个字符的单词，按照单字符逻辑处理
                if (strlen(word) == 1) {
                	unsigned char ch = (unsigned char)temp_character[0]; // 第一个字符
					// ASCII 字符处理
                    int asciiVal = (int)ch;
                    character_Word[count] = asciiVal; // 存储 ASCII 值
                } else {
                    // 处理两个及以上字符的单词
                    for (int k = 0; k < wordCount; k++) {
                        if (strcmp(word, sortedWords[k].word) == 0) {
                            int id = sortedWords[k].id;
                            character_Word[count] = id;
                            break;
                        }
                    }
                }
            } else if ((unsigned char)temp_character[0] < 0x80) {
                // 处理 ASCII 字符（与 encodeFile 保持一致）
                unsigned char ch = (unsigned char)temp_character[0];
                // ASCII 字符处理
                int asciiVal = (int)ch;
                character_Word[count] = asciiVal; // 存储 ASCII 值
            } else if ((unsigned char)temp_character[0] >= 0x80) {
				// 可能是 GBK 或其他双字节汉字
				unsigned char ch = (unsigned char)temp_character[0];
                unsigned char nextCh = (unsigned char)temp_character[1];
                if ((ch >= 0x80 && ch <= 0xFE) && (nextCh >= 0x40 && nextCh <= 0xFE)) {
                    int gbkChar = (ch << 8) | nextCh; // 将两个字节组合为一个整型值
                    character_Word[count] = gbkChar;      // 存储 GBK 编码值
                }
            }
            count++;
        }
    }

    fclose(file);
}