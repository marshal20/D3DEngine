#pragma once
#include <string>
#include "input.h"
#include "pointerutil.h"


struct WindowOptions
{
	bool fullscreen;
	bool border;
	bool cursor;
	bool resizable;
	bool centered;

	int x;
	int y;
	int width; // 0 for fullwidth
	int height; // 0 for fullheight
};

class Window
{
public:
	
	Window();
	Window(const Window& other) = delete;
	~Window();

	void init(const std::string& name = "UNNAMED", const WindowOptions* options = 0);
	void cleanup();

	void update();
	void close();
	bool isClosed();

	void setInputSystem(InputSystem* inputsys);
	void setFullscreenState(bool enabled);

	bool HandleMessage(unsigned int umessage, unsigned int wparam);

private:
	friend class RenderDevice;
	void* getNativeHandle();

private:
	struct NativeHandle;
	ImplPtr<NativeHandle> m_handle;

	InputSystem* m_inputsystem = nullptr;
	WindowOptions m_options;
	bool m_closed = true;
	int m_screenWidth, m_screenHeight;
};
