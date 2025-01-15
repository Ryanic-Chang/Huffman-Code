// frequency.h
#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "linked_list.h"

#define MAX_CHAR 66536  // 适合ASCII和GBK字符
#define BUFFER_SIZE 10240 // 缓冲区大小

void calculateFrequency(const char* filename, ListNode** charList);

#endif // FREQUENCY_H
