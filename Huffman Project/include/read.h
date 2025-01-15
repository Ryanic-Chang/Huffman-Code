#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_COUNT 3200

void readFile(const char *filename, int character[]);
void readFile_Word(const char *filename, int character_Word[]);

#endif
