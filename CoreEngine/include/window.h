#pragma once
#include <string>

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

		void setPosition(const math::Vec2<int>& position);
		void setSize(const math::Vec2<int>& size);

		math::Vec2<int> getPosition();
		math::Vec2<int> getSize();

	private:
		void updateSizePosition();

	private:
		math::Vec2<int> m_size;
		math::Vec2<int> m_position;

	};
}