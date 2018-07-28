#pragma once
#include <ostream>

namespace logger
{
	extern void setTarget(std::ostream& target);
	extern void log(const char* msg);
	extern void customLog(const char* msg, const char* file_name, int line_number);

} // log