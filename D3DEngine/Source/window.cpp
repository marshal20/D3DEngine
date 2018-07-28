#include "window.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

const WindowOptions Window::WIND_OPT_DEF = { false, true, true, false,true, 0, 0, 800, 600, "UNNAMED" };

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

Window::Window()
{


}

Window::~Window()
{

}

void Window::init(const WindowOptions& options)
{
	m_options = options;
	m_closed = false;

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	m_application_name = new wchar_t[m_options.window_name.length() + 1];
	ZeroMemory((char*)m_application_name, (m_options.window_name.length() + 1) * sizeof(wchar_t));
	mbstowcs(m_application_name, m_options.window_name.c_str(), m_options.window_name.length());

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

	// Determine the resolution of the clients desktop screen.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_options.fullscreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = m_options.x;
		posY = m_options.y;
	}
	else
	{
		// If windowed then set it to resolution.
		screenWidth = m_options.width;
		screenHeight = m_options.height;

		// Place the window in the middle of the screen.
		if (m_options.centered)
		{
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}	
	}

	// Set the style.
	int window_width = 0;
	int window_height = 0;
	DWORD window_style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (m_options.resizable) window_style = WS_OVERLAPPEDWINDOW;
	if (!m_options.border || m_options.fullscreen) window_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	// Calculate windows size with style.
	{
		RECT wr = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRect(&wr, window_style, false);

		window_width = wr.right - wr.left;
		window_height = wr.bottom - wr.top;

		printf("%d %d\n", window_width, window_height);
	}

	m_hwnd = CreateWindow(m_application_name, m_application_name,
		window_style,
		posX, posY, window_width, window_height, NULL, NULL, m_hinstance, NULL);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	if (!m_options.cursor) ShowCursor(false);

	return;
}

void Window::destroy()
{
	if (m_hwnd)
	{
		DestroyWindow(m_hwnd);
		UnregisterClass(m_application_name, m_hinstance);
		m_hwnd = 0;
	}

	if (m_application_name)
	{
		delete[] m_application_name;
		m_application_name = 0;
	}
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

bool Window::isClosed()
{
	return m_closed;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		//return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}