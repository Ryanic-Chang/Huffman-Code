#include "linked_list.h"
#include <stdlib.h>

#include <stdio.h>

ListNode nodePool[NODE_POOL_SIZE]; // 创建节点池
int poolIndex = 0; // 指示池中下一个空闲节点的位置

// 创建一个新的字符节点
ListNode* createNode(int character) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (!newNode) {
        perror("Unable to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    newNode->character = character;
    newNode->frequency = 1;
    newNode->next = NULL;
    return newNode;
}

// 在链表中查找或添加字符节点
void addOrUpdateNode(ListNode** head,int character) {
    ListNode* current = *head;

    // 查找字符节点
    while (current != NULL) {
        if (current->character == character) {
            current->frequency++;
            return;
        }
        current = current->next;
    }

    // 如果未找到，创建新节点并添加到链表开头
    ListNode* newNode = createNode(character);
    newNode->next = *head;
    *head = newNode;

    // 确保链表的最后一个节点指向 NULL
    current = newNode; // 从新节点开始检查
    while (current->next != NULL) {
        current = current->next; // 遍历到链表的最后一个节点
    }
    current->next = NULL; // 最后一个节点的 next 指向 NULL
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
{
    if(l1 == NULL && l2 == NULL)    return NULL;
    if(l1 && l2 == NULL)    return l1;
    if(l1 == NULL && l2)    return l2;

    ListNode* head = NULL;
    ListNode* p = NULL;

    while(l1 && l2)
    {
        if(l1->frequency > l2->frequency)
        {
            ListNode* temp = l1;
            l1 = l2;
            l2 = temp;
        }
        else if(l1->frequency == l2->frequency && l1->character > l2->character)
        {
            ListNode* temp = l1;
            l1 = l2;
            l2 = temp;
        }

        if(head)
        {
            p->next = l1;
            p = p->next;
            l1 = l1->next;
        }
        else
        {
            head = l1;
            p = head;
            l1 = l1->next;
        }
    }

    if(l1) p->next = l1;
    else p->next = l2;

    return head;
}

ListNode* _sort(ListNode* start, ListNode* end)
{
    if(start == NULL || start == end || start->next == NULL)    return start;

    ListNode* l1 = start;
    ListNode* l2 = start->next;

    while(l2 != end)
    {
        l1 = l1->next;
        l2 = l2->next;
        if(l2 == end) break;
        l2 = l2->next;
    }

    ListNode* secondStart = l1->next;
    l1->next = NULL;

    ListNode* h1 = _sort(start,l1);
    ListNode* h2 = _sort(secondStart, end);
    ListNode* h = mergeTwoLists(h1,h2);
    return h;
}

ListNode* sortList(ListNode* head)
{
    if(head == NULL || head->next == NULL)  return head;
    return _sort(head,NULL);
}

int countList(ListNode* head)
{
    int cnt = 0;
    while(head != NULL)
    {
        head = head->next;
        cnt++;
    }
    return cnt;
}

// 释放链表的内存
void freeList(ListNode* head) {
    ListNode* current = head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void printCharList(ListNode* head) {
    ListNode* current = head;
    int cnt = 0;
    while (current != NULL) {
        cnt++;
        printf("Character: %u, Frequency: %d\n", current->character, current->frequency);
        current = current->next;
    }
    printf("%d\n",cnt);
}

ListNode* getNodeFromPool() {
    if (poolIndex < NODE_POOL_SIZE) {
        return &nodePool[poolIndex++];
    }
    // 如果池中没有空闲节点，可以选择扩展池或报告错误
    return NULL;
}

// 删除链表中字符为a到z和A到Z的节点
void deleteAlphaNodes(ListNode** head) {
    ListNode* current = *head;
    ListNode* prev = NULL;

    while (current != NULL) {
        // 检查当前节点的字符是否为a-z或A-Z
        if ((current->character >= 'a' && current->character <= 'z') || 
            (current->character >= 'A' && current->character <= 'Z')) {
            
            // 如果删除的是头节点
            if (prev == NULL) {
                *head = current->next; // 更新头节点
            } else {
                prev->next = current->next; // 更新前一个节点的next
            }

            current = current->next; // 移动到下一个节点
        } else {
            prev = current;  // 移动prev指针
            current = current->next;  // 移动current指针
        }
    }
}


void updateCharList(ListNode** head, WordFrequency* sortedWords) {
    for (int i = 0; i < wordCount; i++) {
        WordFrequency currentWord = sortedWords[i];

        // 如果单词为空或者单个字母（a-z 或 A-Z）
        if (strlen(currentWord.word) == 1 && 
            ((currentWord.word[0] >= 'a' && currentWord.word[0] <= 'z') || 
            (currentWord.word[0] >= 'A' && currentWord.word[0] <= 'Z'))) {
            ListNode* newNode = getNodeFromPool();
            if (newNode) {
                newNode->character = (int)(currentWord.word[0]);
                newNode->frequency = currentWord.count;
                newNode->next = *head;
                *head = newNode;
            }
            continue;
        }

        // 从池中获取一个节点
        ListNode* newNode = getNodeFromPool();
        if (newNode) {
            newNode->character = currentWord.id;
            newNode->frequency = currentWord.count;
            newNode->next = *head;
            *head = newNode;
        }
    }
    
    // 对链表排序（根据频率和字符编号）
    *head = sortList(*head);
}
