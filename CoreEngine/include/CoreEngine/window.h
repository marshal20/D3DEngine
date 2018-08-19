#pragma once
#include <string>
#include <memory>

#include "math/vec.h"

namespace ce
{
	class Window
	{
	public:
		Window();
		~Window();

		void init(const std::string& name, const math::Vec2<int>& size);
		void cleanup();

		void update();
		void show();
		void hide();
		void close();

		void setPosition(const math::Vec2<int>& position);
		void setSize(const math::Vec2<int>& size);

		math::Vec2<int> getPosition() const;
		math::Vec2<int> getSize() const;
		bool isClosed() const;

	private:
		void setSizePosition();

	private:
		std::string m_name;
		math::Vec2<int> m_size;
		math::Vec2<int> m_position;
		bool m_close = false;

		struct WindowImpl;
		std::unique_ptr<WindowImpl> m_wimpl;
	};
}