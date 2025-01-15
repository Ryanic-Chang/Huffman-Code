#include "huffman.h"
#include "word_count.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void initHuffmanTree(HuffmanTreeNode root[],ListNode* charList,int numNode)
{
    for(int i = 0;i < 2*numNode;i++)
        root[i].parent = root[i].left = root[i].right = -1;

    ListNode* current = charList;
    for(int i = 0;i < numNode;i++)
    {
        root[i].character = current->character;
        root[i].frequency = current->frequency;
        current = current->next;
    }
}

void createHuffmanTree(HuffmanTreeNode root[],int numNode)
{
    if(numNode <= 1) return;

    for(int i = numNode;i <= 2*numNode-1;i++)
    {
        int min1_fre = 1e9;
        int min2_fre = 1e9;
        int l = -1;
        int r = -1;
        for(int j = 0;j < i;j++)
        {
            if(root[j].frequency < min1_fre && root[j].parent == -1)
            {
                min2_fre = min1_fre;
                r = l;
                min1_fre = root[j].frequency;
                l = j;
            }
            else if(root[j].frequency < min2_fre && root[j].parent == -1)
            {
                min2_fre = root[j].frequency;
                r = j;
            }
        }

        root[l].parent = root[r].parent = i;
        root[i].left = l;
        root[i].right = r;
        root[i].frequency = root[l].frequency+root[r].frequency;
        root[i].character = -1;
    }
}

void createHuffmanCode(HuffmanTreeNode root[], char ***code, int numNode) {
    *code = (char **)malloc(sizeof(char *) * numNode);  // 修改为 *code

    char *cd = (char *)malloc(sizeof(char) * (numNode + 1));  // 用于临时存储编码
    cd[numNode] = '\0';  // 结束符

    for (int i = 0; i < numNode; i++) {
        int start = numNode;  // 注意调整起始位置
        int c = i;
        int f = root[c].parent;

        // 生成从叶节点到根节点的哈夫曼编码
        while (f != -1) {
            start--;
            cd[start] = (root[f].left == c) ? '0' : '1';
            c = f;
            f = root[c].parent;
        }
        start++;
        // 为当前字符节点分配内存并复制编码
        (*code)[i] = (char *)malloc(sizeof(char) * (numNode - start));
        strcpy((*code)[i], &cd[start]);
    }
    
    free(cd);
}

void storeHuffmanCode(const char* filename, char** code, HuffmanTreeNode root[], int numNode) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 遍历每个字符并将其编码写入文件
    for (int i = 0; i < numNode; i++) {
        // 判断是否是ASCII字符还是GBK字符
        if (root[i].character < 0x80) {
            // 打印特殊字符
            if (root[i].character == '\n') {
                fprintf(file, "Character: '\\n' Code: %s\n", code[i]);
            } else if (root[i].character == '\r') {
                fprintf(file, "Character: '\\r' Code: %s\n", code[i]);
            } else if (root[i].character == '\t') {
                fprintf(file, "Character: '\\t' Code: %s\n", code[i]);
            } else {
                fprintf(file, "Character: '%c' Code: %s\n", root[i].character, code[i]);
            }
        } else if (root[i].character > 0x8000) {
            unsigned char firstByte = (root[i].character >> 8) & 0xFF;
            unsigned char secondByte = root[i].character & 0xFF;
            fprintf(file, "Character: '%c%c' Code: %s\n", firstByte, secondByte, code[i]);
        } else {
            int loc;
            for (int j = 0;j < wordCount;j++)
                if(root[i].character == sortedWords[j].id) {
                    loc = j;
                    break;
                }
            fprintf(file, "Character: '%s' Code: %s\n",sortedWords[loc].word, code[i]);
        }
    }

    fclose(file);
}