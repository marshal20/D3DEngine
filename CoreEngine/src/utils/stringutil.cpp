#include "stringutil.h"

#include <stdlib.h>  
#include <string.h>  

#include <string>
#include <iostream>
#include <Windows.h>

namespace ce
{
	namespace util
	{
		std::string utf16_to_utf8(const std::wstring& utf16_str)
		{
			std::string utf8_str;
			int utf8_size;
			char* buffer;

			utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, 0, 0, 0, 0);

			if (utf8_size > 0)
			{
				buffer = new char[utf8_size];
				WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, &buffer[0], utf8_size, 0, 0);
				utf8_str = buffer;
				delete[] buffer;
			}

			return utf8_str;
		}

		std::wstring utf8_to_utf16(const std::string& utf8_str)
		{
			std::wstring utf16_str;
			int utf16_size;
			wchar_t* buffer;

			utf16_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, 0, 0);

			if (utf16_size > 0)
			{
				buffer = new wchar_t[utf16_size];
				MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, buffer, utf16_size);
				utf16_str = buffer;
				delete[] buffer;
			}

			return utf16_str;
		}

	}
}