#pragma once
#include "window.h"
#include "texture.h"
#include "states.h"

#include <string>
#include <vector>
#include <memory>

#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

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
	RenderDevice(const OutputMode& outputmode, Window& outputWindow);
	~RenderDevice();

	void beginScene(float r, float g, float b, float a);
	void endScene();

	void setViewport(int width, int height);
	void setRestrizerOptions(const RestrizerOptions& resOpt);
	void setFullscreenState(bool enabled);
	float getAspectRatio() const;

	static AdapterInfo getAdapterInfo();
	static std::vector<OutputMode> getOutputModes();
	static OutputMode matchOutputMode(unsigned int width, unsigned int height);

private:
	friend class Window;
	void resize(int width, int height);

private:
	InterPtr<ID3D11Device> m_pDevice;
	InterPtr<ID3D11DeviceContext> m_pContext;
	InterPtr<IDXGISwapChain> m_pSwapchain;

	struct RenderDeviceBuffers;
	std::unique_ptr<RenderDeviceBuffers> m_buffers;
	std::unique_ptr<DepthStencilTexture> m_pRenderTargetDepthStencil;
	std::unique_ptr<DepthStencilState> m_pDepthStencilState;

	bool m_vsync_enabled = false;
	bool m_fullscreen = false;
	OutputMode m_outputmode;
};