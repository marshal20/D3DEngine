#pragma once

#include "../math/vec2.h"
#include "../math/vec4.h"

namespace ce
{
	struct Sprite
	{
		math::Vec2<float> position;
		math::Vec4<float> color;

		Sprite()
			: position({ 0.0f, 0.0f }), color({ 0.0f, 0.0f, 0.0f, 1.0f })
		{}

		Sprite(const math::Vec2<float>& position_, const math::Vec4<float>& color_)
			: position(position_), color(color_)
		{}
	};
}