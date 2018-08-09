#pragma once
#include <string>
#include <memory>

#include "input.h"
#include "pointerutil.h"

typedef HWND__* HWND;
typedef HINSTANCE__* HINSTANCE;

class Window
{
public:
	struct Options
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

public:
	
	Window(const std::string& name = "UNNAMED", const Options& options = windowoptions_DEF);
	Window(const Window& other) = delete;
	~Window();

	void update();
	void close();
	bool isClosed();

	void setInputSystem(InputSystem* inputsys);
	void setFullscreenState(bool enabled);
	void setLockMouse(bool lock);

	bool HandleMessage(unsigned int umessage, unsigned int wparam, long lparam);

private:
	friend class RenderDevice;
	HWND getNativeHandle();

private:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hinstance = nullptr;
	wchar_t* m_application_name = nullptr;

	InputSystem* m_inputsystem = nullptr;
	Options m_options;
	bool m_closed = true;
	int m_screenWidth, m_screenHeight;
	bool m_mouseLock = false;

	static const Options windowoptions_DEF;
};
