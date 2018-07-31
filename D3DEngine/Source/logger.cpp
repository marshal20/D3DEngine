#include "logger.h"

#include <iostream>

namespace logger
{
	std::ostream* out = &std::cout;

	void setTarget(std::ostream& target)
	{
		out = &target;
	}

	void log(const char* msg)
	{
		*out << msg << std::endl;
	}

	void customLog(const char* msg, int code, const char* file_name, int line_number)
	{
		*out << msg << "\t code: 0x" << (void*)code << "\n '----> " << file_name << ":" << line_number << "\n";
	}

} // log