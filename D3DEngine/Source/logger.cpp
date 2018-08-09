#include "logger.h"

#include <iostream>
#include <string>

Logger* Logger::MainLogger = nullptr;

void Logger::LOG(int level, const char* msg)
{
	if (MainLogger)
		MainLogger->log(level, msg);
}

void Logger::CUSTOMLOG(int level, const char* msg, int code)
{
	std::string custom_log_msg = std::string(msg) + std::string("   code: 0x") + std::to_string(code);
	LOG(level, custom_log_msg.c_str());
}


void Logger::log(int level, const char* msg)
{
	if (level > m_loglevel) return;
	for (std::ostream* out : m_outs)
		*out << msg << std::endl;
}

void Logger::setLevel(int level)
{
	m_loglevel = level;
}

void Logger::addOut(std::ostream* out)
{
	m_outs.push_back(out);
}
