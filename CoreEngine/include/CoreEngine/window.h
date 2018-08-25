#pragma once
#include <string>
#include <memory>

#include "math/vec.h"

namespace ce
{
	class WindowImpl;

	class Window
	{
	public:
		Window();
		Window(const Window& other) = delete;
		~Window();

		void init(const std::string& name, const math::Vec2<int>& size);
		void cleanup();

		void update();
		void show();
		void hide();
		void close();
		void maxmize();
		void minimize();

		void set_name(const std::string& name);
		void set_position(const math::Vec2<int>& position);
		void set_size(const math::Vec2<int>& size);
		void set_border(bool enable);
		void set_resizable(bool enable);

		bool is_closed() const;
		std::string get_name() const;
		math::Vec2<int> get_position() const;
		math::Vec2<int> get_size() const;
		static math::Vec2<int> get_screen_size();

	private:
		std::unique_ptr<WindowImpl> m_impl;
	};
}