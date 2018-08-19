#include <CoreEngine/window.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <map>

#include "stringutil.h"

// for reference:
// https://msdn.microsoft.com/en-us/library/bb384843.aspx

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

namespace ce
{
	struct Window::WindowImpl
	{
		HWND hwnd;
		HINSTANCE hInstance;
		wchar_t* szTitle;
	};

	math::Vec2<int> getScreenSize();
	std::map<HWND, Window*> g_hwndWindowMap;

	Window::Window()
	{
		m_wimpl = std::make_unique<WindowImpl>();
	}

	Window::~Window()
	{
	}

	void Window::init(const std::string& name, const math::Vec2<int>& size)
	{
		WNDCLASSEX wcex;

		m_name = name;
		m_size = size;

		m_position = (getScreenSize() - m_size) / 2;

		m_wimpl->hInstance = GetModuleHandle(NULL);
		m_wimpl->szTitle = util::create_wcharstr(name.c_str());

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_wimpl->hInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_wimpl->szTitle;
		wcex.hIconSm = wcex.hIcon;

		if (!RegisterClassEx(&wcex))
		{
			// TODO: error message.
		}

		m_wimpl->hwnd = CreateWindow(
			m_wimpl->szTitle,
			m_wimpl->szTitle,
			WS_OVERLAPPEDWINDOW,
			m_position.x, m_position.y,
			size.x, size.y,
			NULL,
			NULL,
			m_wimpl->hInstance,
			NULL
		);

		if (!m_wimpl->hwnd)
		{
			// TODO: error message.
		}

		ShowWindow(m_wimpl->hwnd, SW_SHOW);
		UpdateWindow(m_wimpl->hwnd);
	
		g_hwndWindowMap.insert(std::make_pair(m_wimpl->hwnd, this));
	}

	void Window::cleanup()
	{
		UnregisterClass(m_wimpl->szTitle, m_wimpl->hInstance);

		if (m_wimpl->szTitle)
		{
			delete[] m_wimpl->szTitle;
		}

		g_hwndWindowMap.erase(m_wimpl->hwnd);
	}

	void Window::update()
	{
		MSG msg;

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

	void Window::show()
	{
		ShowWindow(m_wimpl->hwnd, SW_SHOW);
	}

	void Window::hide()
	{
		ShowWindow(m_wimpl->hwnd, SW_HIDE);
	}

	void Window::close()
	{
		m_close = true;
	}

	void Window::setPosition(const math::Vec2<int>& position)
	{
		m_position = position;
		m_size = getSize();
		setSizePosition();
	}

	void Window::setSize(const math::Vec2<int>& size)
	{
		m_size = size;
		m_position = getPosition();
		setSizePosition();
	}

	math::Vec2<int> Window::getPosition() const
	{
		RECT winrc;

		GetWindowRect(m_wimpl->hwnd, &winrc);
		return { winrc.left, winrc.top };
	}

	math::Vec2<int> Window::getSize() const
	{
		RECT winrc;

		GetWindowRect(m_wimpl->hwnd, &winrc);
		return { winrc.right - winrc.left, winrc.bottom - winrc.top };
	}

	bool Window::isClosed() const
	{
		return m_close;
	}

	void Window::setSizePosition()
	{
		SetWindowPos(m_wimpl->hwnd, NULL, m_position.x, m_position.y, m_size.x, m_size.y, NULL);
	}

	// HELPER FUNCTIONS

	math::Vec2<int> getScreenSize()
	{
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		return { x, y };
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ce::Window* wind;

	auto foundWind = ce::g_hwndWindowMap.find(hWnd);
	wind = (foundWind != ce::g_hwndWindowMap.end()) ? foundWind->second : nullptr;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}















/*

// GT_HelloWorldWin32.cpp  
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c  

 
#include <stdlib.h>  
#include <string.h>  
#include <tchar.h>  

// Global variables  

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable  

					   // The parameters to CreateWindow explained:  
					   // szWindowClass: the name of the application  
					   // szTitle: the text that appears in the title bar  
					   // WS_OVERLAPPEDWINDOW: the type of window to create  
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
					   // 500, 100: initial size (width, length)  
					   // NULL: the parent of this window  
					   // NULL: this application does not have a menu bar  
					   // hInstance: the first parameter from WinMain  
					   // NULL: not used in this application  
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
*/

//  
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)  
//  
//  PURPOSE:  Processes messages for the main window.  
//  
//  WM_PAINT    - Paint the main window  
//  WM_DESTROY  - post a quit message and return  
//  
//  

