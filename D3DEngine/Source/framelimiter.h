#pragma once

#include <memory>

class FrameLimiter
{
public:
	FrameLimiter(int targetFramrate = 60);
	~FrameLimiter();

	void setTargetFrameRate(int framerate);

	void start();
	void end();

	double getLastFramTime();

private:
	struct FrameLimiterBuffers;
	std::unique_ptr<FrameLimiterBuffers> m_buffers;
};