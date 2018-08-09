#pragma once
#include <ostream>
#include <vector>

class Logger
{
public:
	static Logger* MainLogger;

public:
	Logger();
	Logger(const Logger& other) = default;
	~Logger();

	static void LOG(int level, const char* msg);
	static void CUSTOMLOG(int level, const char* msg, int code);

	void log(int level, const char* msg);

	void setLevel(int level);
	void addOut(std::ostream* out);

private:
	std::vector<std::ostream*> m_outs;
	int m_loglevel = 4;

};