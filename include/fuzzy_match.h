#ifndef FUZZY_MATCH_H
#define FUZZY_MATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 函数声明
int levenshteinDistance(const char* s1, const char* s2);
void fuzzyMatchInFile(const char* pattern);

#endif // FUZZY_MATCH_H
