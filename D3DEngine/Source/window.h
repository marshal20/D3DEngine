#pragma once
#include <string>
#include "input.h"

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

	bool HandleMessage(unsigned int umessage, unsigned int wparam);

private:
	friend class RenderDevice;
	void* getNativeHandle() const;

private:
	struct NativeHandle;
	NativeHandle* m_handle = nullptr;

	WindowOptions m_options;
	bool m_closed = true;
	InputSystem* m_inputsystem = nullptr;
	int m_screenWidth, m_screenHeight;
};
