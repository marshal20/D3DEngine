#pragma once
#include <string>
#include <Windows.h>
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
	void close();
	bool isClosed();

	void setInputSystem(InputSystem* inputsys);

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hinstance = nullptr;
	wchar_t* m_application_name = nullptr;
	WindowOptions m_options;
	bool m_closed = true;
	InputSystem* m_inputsystem = nullptr;

public:
	static const WindowOptions WIND_OPT_DEF;
	static Window* main_window;
};
