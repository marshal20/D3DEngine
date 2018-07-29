#pragma once
#include <string>
#include <vector>
#include "window.h"

struct AdapterInfo
{
	std::string Description;
	unsigned int VendorId;
	unsigned int DeviceId;
	unsigned int SubSysId;
	unsigned int Revision;
	size_t DedicatedVideoMemory;
	size_t DedicatedSystemMemory;
	size_t SharedSystemMemory;
};

struct Rational
{
	unsigned int Numerator;
	unsigned int Denominator;
};

struct OutputMode
{
	unsigned int width;
	unsigned int height;
	Rational RefreshRate;
};

class Device
{
public:
	Device();
	~Device();
	void init(const OutputMode& outputmode, const Window& outputWindow);
	void cleanup();

	void beginScene(float r, float g, float b, float a);
	void endScene();

	void setViewport(const int width, const int height);

	static AdapterInfo getAdapterInfo();
	static std::vector<OutputMode> getOutputModes();
	static OutputMode getOutputMode(unsigned int width, unsigned int height);

private:
	struct D3D11Impl;
	D3D11Impl* m_impl = nullptr;

	bool m_vsync_enabled = false;
	int numerator = 0;
	int denominator = 1;
};