#pragma once
#include <stddef.h>
#include "typedefs.cpp"

char* StrConcat (const char* str1, const char* str2, const char delim = ' ') {
    const size_t len1 = sizeof(str1);
    const size_t len2 = sizeof(str2);

    string ret = new char[len1 + len2 + 2];

    for (int i = 0; i < len1; i ++) { if (str1[i] != '\0') ret[i] = str1[i]; }
    ret[len1] = delim;
    for (int i = 0; i < len2; i++) ret[len1 + i] = str2[i];
    ret[len1+len2+1] = '\0';

    return ret;
}
