#include "verify.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compareFiles(const char* file1, const char* file2) {
    FILE *f1 = fopen(file1, "rb"); // 以二进制模式打开文件
    FILE *f2 = fopen(file2, "rb");

    if (f1 == NULL || f2 == NULL) {
        perror("Error opening files for comparison");
        return;
    }

    unsigned char buffer1[1024], buffer2[1024]; // 定义缓冲区
    size_t bytesRead1, bytesRead2;

    // 循环比较文件内容
    while ((bytesRead1 = fread(buffer1, 1, sizeof(buffer1), f1)) > 0 &&
           (bytesRead2 = fread(buffer2, 1, sizeof(buffer2), f2)) > 0) {
        if (bytesRead1 != bytesRead2 || memcmp(buffer1, buffer2, bytesRead1) != 0) {
            printf("Files are not identical.\n");
            fclose(f1);
            fclose(f2);
            return;
        }
    }

    // 检查文件是否同时到达结束
    if (feof(f1) && feof(f2)) {
        printf("Files are identical.\n");
    } else {
        printf("Files are not identical.\n");
    }

    fclose(f1);
    fclose(f2);
}
