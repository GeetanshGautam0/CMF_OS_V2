#pragma once
#include <stddef.h>
#include "typedefs.h"
#include "TextPrint.cpp"

int StrLen (const char * str) {
    int i = 0;
    for(; str[i] != 0; i++);
    return i;
}

char * StrConcat (const char * str1, const char * str2, const char delim = ' ') {

	const int len1 = StrLen(str1);
	const int len2 = StrLen(str2);

	// char arr [len1 + len2 + 2];
	// PrintString(IntToStr(sizeof(arr)), ScreenColor, true);
	PrintString(IntToStr(len1), ScreenColor, true);
	PrintString(IntToStr(len2), ScreenColor, true);

	// PrintString("\n\r");

	// for (int i = 0; i < len1; i ++) arr[i] = str1[i];
	// for (int i = 0; i < len2; i ++) arr[len1 + i] = str2[i];
	// arr[len1] = delim;
	// arr[len1 + len2 + 1] = '\0';

	return "";

}