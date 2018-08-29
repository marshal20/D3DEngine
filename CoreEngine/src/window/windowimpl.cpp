#include "windowimpl.h"

#include <map>

#include "..\utils\stringutil.h"

// for reference:
// https://msdn.microsoft.com/en-us/library/bb384843.aspx

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
std::map<HWND, ce::WindowImpl*> g_hwndWindowMap;

namespace ce
{

	WindowImpl::WindowImpl()
	{

	}

	WindowImpl::~WindowImpl()
	{

	}

	void WindowImpl::init(const std::string& name, const math::Vec2<int>& size)
	{
		WNDCLASSEX wcex;

		m_name = name;
		m_size = size;

		m_position = (get_screen_size() - m_size) / 2;

		m_hInstance = GetModuleHandle(NULL);
		m_class_name = util::create_wcharstr(m_name.c_str());

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_class_name;
		wcex.hIconSm = wcex.hIcon;

		if (!RegisterClassEx(&wcex))
		{
			// TODO: error message.
		}

		m_hWnd = CreateWindow(
			m_class_name,
			m_class_name,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			size.x, size.y,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		if (!m_hWnd)
		{
			// TODO: error message.
		}

		show();
		UpdateWindow(m_hWnd);

		g_hwndWindowMap.insert(std::make_pair(m_hWnd, this));
	}

	void WindowImpl::cleanup()
	{
		close();

		if (m_class_name)
		{
			delete[] m_class_name;
			m_class_name = nullptr;
		}

		if (m_hWnd)
		{
			UnregisterClass(m_class_name, m_hInstance);
			g_hwndWindowMap.erase(m_hWnd);
			m_hWnd = nullptr;
		}

		m_hInstance = nullptr;
	}

	LRESULT WindowImpl::handle_msg(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			close();
			break;

		case WM_CLOSE:
			close();
			break;

		default:
			return DefWindowProc(m_hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}

	void WindowImpl::update()
	{
		MSG msg;

		if (m_close) return;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowImpl::close()
	{
		DestroyWindow(m_hWnd);
		m_close = true;
	}

	void WindowImpl::show()
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}

	void WindowImpl::hide()
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	void WindowImpl::maxmize()
	{
		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}

	void WindowImpl::minimize()
	{
		ShowWindow(m_hWnd, SW_MINIMIZE);
	}

	void WindowImpl::set_name(const std::string& name)
	{
		wchar_t* new_title;

		m_name = name;
		new_title = util::create_wcharstr(m_name.c_str());
		SetWindowText(m_hWnd, new_title);
		delete[] new_title;
	}

	void WindowImpl::set_position(const math::Vec2<int>& position)
	{
		m_position = position;
		m_size = get_size();
		set_size_position();
	}

	void WindowImpl::set_position_centered()
	{
		m_size = get_size();
		m_position = (get_screen_size() - m_size) / 2;
		set_size_position();
	}

	void WindowImpl::set_size(const math::Vec2<int>& size)
	{
		m_size = size;
		m_position = get_position();
		set_size_position();
	}

	void WindowImpl::set_border(bool enable)
	{
		set_style_state(m_hWnd, WS_OVERLAPPEDWINDOW, enable);
		show();
	}

	void WindowImpl::set_resizable(bool enable)
	{
		if (enable)
		{
			SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		}
		else
		{
			SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		}
		show();
	}

	bool WindowImpl::is_closed() const
	{
		return m_close;
	}

	std::string WindowImpl::get_name() const
	{
		std::string name;
		char* name_c;
		int length;

		length = GetWindowTextLength(m_hWnd);
		name_c = new char[length+1];
		GetWindowTextA(m_hWnd, name_c, length + 1);
		name = name_c;
		delete[] name_c;
		return name;
	}

	math::Vec2<int> WindowImpl::get_position() const
	{
		RECT winrc;

		GetWindowRect(m_hWnd, &winrc);
		return { winrc.left, winrc.top };
	}

	math::Vec2<int> WindowImpl::get_size() const
	{
		RECT winrc;

		GetWindowRect(m_hWnd, &winrc);
		return { winrc.right - winrc.left, winrc.bottom - winrc.top };
	}

	math::Vec2<int> WindowImpl::get_client_size() const
	{
		RECT winrc;

		GetWindowRect(m_hWnd, &winrc);
		AdjustWindowRect(&winrc, GetWindowLong(m_hWnd, GWL_STYLE), false);
		return { winrc.right - winrc.left, winrc.bottom - winrc.top };
	}

	math::Vec2<int> WindowImpl::get_screen_size()
	{
		int x, y;
		x = GetSystemMetrics(SM_CXSCREEN);
		y = GetSystemMetrics(SM_CYSCREEN);
		return { x, y };
	}

	HWND WindowImpl::get_hWnd() const
	{
		return m_hWnd;
	}

	// PRIVATE

	void WindowImpl::set_size_position()
	{
		SetWindowPos(m_hWnd, NULL, m_position.x, m_position.y, m_size.x, m_size.y, NULL);
	}

	void WindowImpl::set_style_state(HWND hWnd, long style, bool enable)
	{
		long new_style;

		new_style = GetWindowLong(hWnd, GWL_STYLE);
		if (enable)
		{
			new_style = new_style | style;
		}
		else
		{
			new_style = new_style & (~style);
		}
		SetWindowLong(hWnd, GWL_STYLE, new_style);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ce::WindowImpl* wind;

	auto foundWind = g_hwndWindowMap.find(hWnd);
	wind = (foundWind != g_hwndWindowMap.end()) ? foundWind->second : nullptr;

	if (wind)
	{
		return wind->handle_msg(message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}