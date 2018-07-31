#pragma once
#include <string>
#include <vector>
#include "window.h"
#include <memory>


struct RenderDeviceImpl;

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

struct RestrizerOptions
{
	enum class CullMode	{ Front = 1, Back, None };
	enum class FillMode { Solid = 1, Wireframe, };

	CullMode cullmode;
	FillMode fillmode;
	bool frontCounterClockwise;
	bool multisampleEnable;
	bool scissorEnable;
};

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice();
	void init(const OutputMode& outputmode, Window& outputWindow);
	void cleanup();

	void beginScene(float r, float g, float b, float a);
	void endScene();

	void setViewport(int width, int height);
	void setRestrizerOptions(const RestrizerOptions& resOpt);
	void setFullscreenState(bool enabled);

	static AdapterInfo getAdapterInfo();
	static std::vector<OutputMode> getOutputModes();
	static OutputMode matchOutputMode(unsigned int width, unsigned int height);

private:
	friend class Renderer;
	friend class Shader;
	friend class Buffer;
	std::shared_ptr<RenderDeviceImpl> getImplementation();

private:
	std::shared_ptr<RenderDeviceImpl> m_impl;
	struct RenderDeviceBuffers;
	std::unique_ptr<RenderDeviceBuffers> m_buffers;

	bool m_vsync_enabled = false;
	bool m_fullscreen = false;
	OutputMode m_outputmode;
};