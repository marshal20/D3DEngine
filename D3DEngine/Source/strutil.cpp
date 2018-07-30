#include "strutil.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>

wchar_t* create_wcharstr(const char* src)
{
	wchar_t* buffer = 0;

	buffer = new wchar_t[strlen(src) + 1];
	ZeroMemory((char*)buffer, (strlen(src) + 1) * sizeof(wchar_t));
	mbstowcs(buffer, src, strlen(src));

	return buffer;
}
