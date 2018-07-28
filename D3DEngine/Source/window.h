#pragma once
#include <string>
#include <Windows.h>

struct WindowOptions
{
	bool fullscreen;
	bool border;
	bool cursor;
	bool resizable;
	bool centered;

	int x;
	int y;
	int width;
	int height;

	std::string window_name;
};



class Window
{
public:
	Window();
	Window(const Window& other) = delete;
	~Window();
	void init(const WindowOptions& options = Window::WIND_OPT_DEF);
	void destroy();
	void update();
	bool isClosed();

public:
	static const WindowOptions WIND_OPT_DEF;

private:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hinstance = nullptr;
	wchar_t* m_application_name = nullptr;
	WindowOptions m_options;
	bool m_closed = true;
};
