#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "word_count.h"
#define NODE_POOL_SIZE 2000 // 假设最大节点数为2000

typedef struct ListNode {
    int character;
    int frequency;
    struct ListNode* next;
} ListNode;

extern ListNode nodePool[NODE_POOL_SIZE]; // 创建节点池
extern int poolIndex; // 指示池中下一个空闲节点的位置

ListNode* createNode(int character);
void addOrUpdateNode(ListNode** head,int character);
ListNode* sortList(ListNode* head);
int countList(ListNode* head);
void freeList(ListNode* head);
// 声明打印链表的函数
void printCharList(ListNode* head);
void deleteAlphaNodes(ListNode** head);
void updateCharList(ListNode** head, WordFrequency* sortedWords);


#endif // LINKED_LIST_H
