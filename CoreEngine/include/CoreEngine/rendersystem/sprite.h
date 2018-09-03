#pragma once

#include "../math/vec2.h"
#include "../math/vec4.h"

namespace ce
{
	class Texture2D;
	
	struct Sprite
	{
		math::Vec2<float> position;
		math::Vec2<float> size;
		math::Vec4<float> color;
		Texture2D* texture;
	};
}