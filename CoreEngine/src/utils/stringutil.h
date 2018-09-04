#pragma once

#include <string>

namespace ce
{
	namespace util
	{
		extern std::string utf16_to_utf8(const std::wstring& utf16_str);
		extern std::wstring utf8_to_utf16(const std::string& utf8_str);
	}
}
