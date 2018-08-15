#include "window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include "checks.h"
#include "strutil.h"

#include <map>


const Window::Options Window::windowoptions_DEF = { false, true, true, false, true, 0, 0, 800, 600 };

std::map<HWND, Window*> g_windowmap;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
void setScreenRes(int width, int height);
DWORD calcStyle(const Window::Options& options);
void calcScreenDimentions(const Window::Options& options, int& width, int& height);
void calcWindowPos(const Window::Options& options, int& x, int& y, int& width, int& height);
void calcWindowDimentions(DWORD style, int screenWidth, int screenHeight, int& width, int& height);
void setMousePosRelative(HWND hwnd, int x, int y);


Window::Window(const std::string& name, const Options& options) :
	m_options(options), m_closed(false)
{
	WNDCLASSEX wc;
	DWORD window_style;
	int window_width; int window_height;
	int window_x, window_y;

	m_application_name = create_wcharstr(name.c_str());

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; //WndProc
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_application_name;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	calcScreenDimentions(m_options, m_screenWidth, m_screenHeight);
	if (m_options.fullscreen)
		setScreenRes(m_screenWidth, m_screenHeight);

	// calculations
	window_style = calcStyle(m_options);
	calcWindowDimentions(window_style, m_screenWidth, m_screenHeight, window_width, window_height);
	calcWindowPos(m_options, window_x, window_y, window_width, window_height);

	m_hwnd = CreateWindow(m_application_name, m_application_name,
		window_style,
		window_x, window_y, window_width, window_height, NULL, NULL, m_hinstance, NULL);

	if (!m_hwnd)
	{
		checks::ENGINE_ERR("can't initialize window.");
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	if (!m_options.cursor) ShowCursor(false);

	g_windowmap.insert(std::make_pair(m_hwnd, this));
}

Window::~Window()
{
	if (m_hwnd) {
		DestroyWindow(m_hwnd);
		UnregisterClass(m_application_name, m_hinstance);
	}

	if (m_application_name) {
		delete[] m_application_name;
	}

	g_windowmap.erase(m_hwnd);
}


void Window::update()
{
	MSG msg;
	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Handle the windows messages.
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		close();
	}

}

void Window::close()
{
	ShowWindow(m_hwnd, HIDE_WINDOW);
	m_closed = true;
}

bool Window::isClosed()
{
	return m_closed;
}

void Window::setInputSystem(InputSystem* inputsys)
{
	m_inputsystem = inputsys;
}

void Window::setFullscreenState(bool enabled)
{
	m_options.fullscreen = enabled;

	if (m_options.fullscreen)
		setScreenRes(m_screenWidth, m_screenHeight);
	else
		ChangeDisplaySettings(NULL, 0);
}

void Window::setLockMouse(bool lock)
{
	m_mouseLock = lock;
	ShowCursor(!m_mouseLock);
	RECT clipRect;
	GetWindowRect(m_hwnd, &clipRect);
	if (m_mouseLock)
		ClipCursor(&clipRect);
	else
		ClipCursor(NULL);
}

bool Window::HandleMessage(unsigned int umessage, unsigned int wparam, long lparam)
{
	int xPos = GET_X_LPARAM(lparam);
	int yPos = GET_Y_LPARAM(lparam);
	static int lastxPos = xPos;
	static int lastyPos = yPos;

	switch (umessage)
	{
	case WM_KEYDOWN:
		if (m_inputsystem) 
			m_inputsystem->keydown((unsigned int)wparam); 
		return true;

	case WM_KEYUP:
		if (m_inputsystem) 
			m_inputsystem->keyup((unsigned int)wparam); 
		return true;

	case WM_MOUSEMOVE:
		if (m_inputsystem)
			m_inputsystem->mousemove(xPos, yPos, xPos - lastxPos, yPos - lastyPos);
		lastxPos = xPos;
		lastyPos = yPos;
		if (m_mouseLock)
		{
			setMousePosRelative(m_hwnd, m_screenWidth / 2, m_screenHeight / 2);
			lastxPos = m_screenWidth / 2;
			lastyPos = m_screenHeight / 2;
		}
		return true;

	default:
		return false;
	}
}

HWND Window::getNativeHandle()
{
	return m_hwnd;
}


// HELPER FUNCTIONS

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		auto found_window = g_windowmap.find(hwnd);
		if (found_window != g_windowmap.end())
		{
			Window* window = found_window->second;

			if (window->HandleMessage(umessage, wparam, lparam))
				return 0;
		}

		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

void setScreenRes(int width, int height)
{
	DEVMODE dmScreenSettings;

	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = (unsigned long)width;
	dmScreenSettings.dmPelsHeight = (unsigned long)height;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
}

DWORD calcStyle(const Window::Options& options)
{
	DWORD style;

	style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	if (options.resizable)
		style = WS_OVERLAPPEDWINDOW;

	if (!options.border || options.fullscreen)
		style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	return style;
}

void calcScreenDimentions(const Window::Options& options, int& width, int& height)
{
	width = options.width;
	height = options.height;

	if (width == 0)
		width = GetSystemMetrics(SM_CXSCREEN);

	if (height == 0)
		height = GetSystemMetrics(SM_CYSCREEN);
}

void calcWindowPos(const Window::Options& options, int& x, int& y, int& width, int& height)
{
	if (options.fullscreen)
	{
		x = 0;
		y = 0;
	}
	if (options.centered)
	{
		x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}
	else
	{
		x = options.x;
		y = options.y;
	}
}

void calcWindowDimentions(DWORD style, int screenWidth, int screenHeight, int& width, int& height)
{
	RECT wr;

	wr = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&wr, style, false);

	width = wr.right - wr.left;
	height = wr.bottom - wr.top;
}

void setMousePosRelative(HWND hwnd, int x, int y)
{
	POINT pos = { x, y };
	ClientToScreen(hwnd, &pos);
	SetCursorPos(pos.x, pos.y);

}
