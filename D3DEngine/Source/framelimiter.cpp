#include "framelimiter.h"

#include <chrono>
#include <thread>


typedef std::chrono::high_resolution_clock::time_point CTimePoint;

struct FrameLimiter::FrameLimiterBuffers
{
	int targetfps;
	double targetFrameTime;
	CTimePoint lastTime;
	CTimePoint curTime;
	double lastFrameTime;
};

FrameLimiter::FrameLimiter(int targetFramrate)
{
	m_buffers = std::make_unique<FrameLimiterBuffers>();

	m_buffers->targetfps = targetFramrate;
	m_buffers->targetFrameTime = 1.0 / (double)targetFramrate;
	m_buffers->curTime = std::chrono::high_resolution_clock::now();
}

FrameLimiter::~FrameLimiter()
{

}

void FrameLimiter::setTargetFrameRate(int framerate)
{
	m_buffers->targetfps = framerate;
	m_buffers->targetFrameTime = 1.0 / (double)framerate;
}

void FrameLimiter::start()
{
}

void FrameLimiter::end()
{
	std::chrono::duration<double> elapsed_seconds;

	m_buffers->lastTime = m_buffers->curTime;
	m_buffers->curTime = std::chrono::high_resolution_clock::now();

	elapsed_seconds = m_buffers->curTime - m_buffers->lastTime;
	m_buffers->lastFrameTime = elapsed_seconds.count();

	double diff = m_buffers->targetFrameTime - m_buffers->lastFrameTime;
	if (diff > 0.001)
	{
		std::chrono::duration<double> toSleep(diff);
		std::this_thread::sleep_for(toSleep);
	}

	m_buffers->curTime = std::chrono::high_resolution_clock::now();
	elapsed_seconds = m_buffers->curTime - m_buffers->lastTime;
	m_buffers->lastFrameTime = elapsed_seconds.count();
}

float FrameLimiter::getLastFramTime()
{
	return m_buffers->lastFrameTime;
}
