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

	void customLog(const char* msg, const char* file_name, int line_number)
	{
		*out << msg << "\n '----> " << file_name << ":" << line_number << "\n";
	}

} // log