#pragma once

namespace ce
{
	namespace math
	{
		template<typename T = float>
		struct Vec2
		{
			T x;
			T y;

			Vec2(const T x_val, const T y_val)
			{
				x = x_val;
				y = y_val;
			}

			Vec2(const Vec2& other)
			{
				x = other.x;
				y = other.y;
			}

			operator= (const Vec2& other)
			{
				x = other.x;
				y = other.y;
			}

			operator+ (const Vec2& other)
			{
				x += other.x;
				y += other.y;
			}

			operator- (const Vec2& other)
			{
				x -= other.x;
				y -= other.y;
			}

			operator* (const Vec2& other)
			{
				x *= other.x;
				y *= other.y;
			}

			operator/ (const Vec2& other)
			{
				x /= other.x;
				y /= other.y;
			}
		};
	}
}
