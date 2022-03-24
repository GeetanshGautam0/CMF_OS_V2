#pragma once
#include <stddef.h>
#include "typedefs.h"
#include "TextPrint.cpp"

int StrLen (const char * str) {
    int i = 0;
    for(; str[i] != 0; i++);
    return i;
}

template<size_t N>
char* StrConcat (const char * str1, const char * str2, char (&array)[N], const char delim = ' ') {

	const int len1 = StrLen(str1);
	const int len2 = StrLen(str2);

	for (int i = 0; i < (len1 >= N ? N : len1); i++) array[i] = str1[i];
	if (len1 < N-1) {
		for (int i = 0; (i + 1 + len1) < N; i++) array[len1+i+1] = str2[i];
		array[len1] = delim;
	}
	array[N] = 0;

	return array;
}