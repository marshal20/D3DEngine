#include "window.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "checks.h"
#include "strutil.h"

const WindowOptions Window::windowoptions_DEF = { false, true, true, false, true, 0, 0, 800, 600 };
Window* main_window = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
void setScreenRes(int width, int height);
DWORD calcStyle(const WindowOptions& options);
void calcScreenDimentions(const WindowOptions& options, int& width, int& height);
void calcWindowPos(const WindowOptions& options, int& x, int& y, int& width, int& height);
void calcWindowDimentions(DWORD style, int screenWidth, int screenHeight, int& width, int& height);

struct Window::NativeHandle
{
	HWND hwnd = nullptr;
	HINSTANCE hinstance = nullptr;
	wchar_t* application_name = nullptr;
};

Window::Window()
{

}

Window::~Window()
{

}

void Window::init(const std::string& name, const WindowOptions& options)
{
	// settingup
	m_options = options;
	m_closed = false;
	main_window = this;

	// local variables
	WNDCLASSEX wc;
	DWORD window_style;
	int window_width; int window_height;
	int window_x, window_y;

	m_handle->application_name = create_wcharstr(name.c_str());

	// Get the instance of this application.
	m_handle->hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; //WndProc
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_handle->hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_handle->application_name;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	calcScreenDimentions(m_options, m_screenWidth, m_screenHeight);
	if(m_options.fullscreen) 
		setScreenRes(m_screenWidth, m_screenHeight);

	// calculations
	window_style = calcStyle(m_options);
	calcWindowDimentions(window_style, m_screenWidth, m_screenHeight, window_width, window_height);
	calcWindowPos(m_options, window_x, window_y, window_width, window_height);

	m_handle->hwnd = CreateWindow(m_handle->application_name, m_handle->application_name,
		window_style,
		window_x, window_y, window_width, window_height, NULL, NULL, m_handle->hinstance, NULL);

	if (!m_handle->hwnd)
	{
		ENGINE_ERROR("can't initialize window.");
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_handle->hwnd, SW_SHOW);
	SetForegroundWindow(m_handle->hwnd);
	SetFocus(m_handle->hwnd);

	// Hide the mouse cursor.
	if (!m_options.cursor) ShowCursor(false);

	return;
}

void Window::cleanup()
{
	if (m_handle->hwnd)
	{
		DestroyWindow(m_handle->hwnd);
		UnregisterClass(m_handle->application_name, m_handle->hinstance);
		m_handle->hwnd = 0;
	}

	if (m_handle->application_name)
	{
		delete[] m_handle->application_name;
		m_handle->application_name = 0;
	}

	m_closed = true;
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
		m_closed = true;
	}

}

void Window::close()
{
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

bool Window::HandleMessage(unsigned int umessage, unsigned int wparam)
{
	switch (umessage)
	{
	/*case WM_ACTIVATEAPP: // leaving this code commented just if i need it back.
	{
		return false;
		if (!m_options.fullscreen) return false;

		if (LOWORD(wparam) == WA_INACTIVE)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		if (LOWORD(wparam) == WA_ACTIVE)
		{
			setScreenRes(m_screenWidth, m_screenHeight);
		}
		return true;
	}*/

	case WM_KEYDOWN:
		if (m_inputsystem) 
			m_inputsystem->keydown((unsigned int)wparam); 
		return true;

	case WM_KEYUP:
		if (m_inputsystem) 
			m_inputsystem->keyup((unsigned int)wparam); 
		return true;

	default:
		return false;
	}
}

void* Window::getNativeHandle()
{
	return (void*)(m_handle->hwnd);
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
		if (main_window->HandleMessage(umessage, wparam))
			return 0;
		
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

DWORD calcStyle(const WindowOptions& options)
{
	DWORD style;

	style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	if (options.resizable)
		style = WS_OVERLAPPEDWINDOW;

	if (!options.border || options.fullscreen)
		style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	return style;
}

void calcScreenDimentions(const WindowOptions& options, int& width, int& height)
{
	width = options.width;
	height = options.height;

	if (width == 0)
		width = GetSystemMetrics(SM_CXSCREEN);

	if (height == 0)
		height = GetSystemMetrics(SM_CYSCREEN);
}

void calcWindowPos(const WindowOptions& options, int& x, int& y, int& width, int& height)
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
