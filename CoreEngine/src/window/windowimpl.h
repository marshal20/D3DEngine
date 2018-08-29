#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include <CoreEngine\math\vec.h>

namespace ce
{
	class WindowImpl
	{
	public:
		WindowImpl();
		WindowImpl(const WindowImpl& other) = delete;
		~WindowImpl();

		void init(const std::string& name, const math::Vec2<int>& size);
		void cleanup();
		LRESULT handle_msg(UINT message, WPARAM wParam, LPARAM lParam);

		void update();
		void close();
		void show();
		void hide();
		void maxmize();
		void minimize();

		void set_name(const std::string& name);
		void set_position(const math::Vec2<int>& position);
		void set_position_centered();
		void set_size(const math::Vec2<int>& size);
		void set_border(bool enable);
		void set_resizable(bool enable);

		bool is_closed() const;
		std::string get_name() const;
		math::Vec2<int> get_position() const;
		math::Vec2<int> get_size() const;
		math::Vec2<int> get_client_size() const;
		static math::Vec2<int> get_screen_size();

		HWND get_hWnd() const;

	private:
		void set_size_position();
		static void set_style_state(HWND hWnd, long style, bool enable);

	private:
		HWND m_hWnd = nullptr;
		HINSTANCE m_hInstance = nullptr;
		wchar_t* m_class_name = nullptr;

		std::string m_name;
		math::Vec2<int> m_size;
		math::Vec2<int> m_position;
		bool m_close = false;
	};
}
