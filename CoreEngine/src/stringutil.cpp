#include "stringutil.h"

#include <stdlib.h>  
#include <string.h>  

namespace ce
{
	namespace util
	{

		wchar_t* create_wcharstr(const char* src)
		{
			size_t size = strlen(src) + 1;
			wchar_t* buffer = new wchar_t[size];

			size_t outSize;
			mbstowcs_s(&outSize, buffer, size, src, size - 1);

			return buffer;
		}

	}
}