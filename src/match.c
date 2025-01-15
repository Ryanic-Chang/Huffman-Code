#include "match.h"
#include <stdio.h>
#include <string.h>

// 段落搜索
void searchParagraph(const char* keyword) {
    FILE *file = fopen("data/SourceDoc.txt", "r");
    if (!file) {
        perror("Unable to open source document");
        return;
    }

    char buffer[10240];
    int paragraphIndex = 1;
    int foundMatch = 0; // 记录是否找到匹配

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, keyword)) {
            printf("Found in paragraph %d: %s\n", paragraphIndex, buffer);
            foundMatch = 1; // 标记为找到匹配
        }
        if (strcmp(buffer, "\n") == 0) { // 段落结束
            paragraphIndex++;
        }
    }

    if (!foundMatch) {
        printf("No matches found for the keyword \"%s\".\n", keyword);
    }

    fclose(file);
}
