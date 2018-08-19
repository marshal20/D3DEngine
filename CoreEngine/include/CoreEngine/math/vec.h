#pragma once

namespace ce
{
	namespace math
	{
		template<typename T>
		struct Vec2
		{
			T x;
			T y;

			Vec2()
			{
				x = static_cast<T>(0);
				y = static_cast<T>(0);
			}

			Vec2(const T x_val, const T y_val)
			{
				x = x_val;
				y = y_val;
			}

			Vec2(const Vec2<T>& other)
			{
				x = other.x;
				y = other.y;
			}

			Vec2<T>& operator= (const Vec2<T>& other)
			{
				x = other.x;
				y = other.y;
				return *this;
			}

			Vec2<T> operator+ (const Vec2<T>& other)
			{
				return { x + other.x, y + other.y };
			}

			Vec2<T> operator- (const Vec2<T>& other)
			{
				return { x - other.x, y - other.y };
			}

			Vec2<T> operator* (const Vec2<T>& other)
			{
				return { x * other.x, y * other.y };
			}

			Vec2<T> operator/ (const Vec2<T>& other)
			{
				return { x / other.x, y / other.y };
			}

			Vec2<T> operator+ (const T other)
			{
				return { x + other, y + other };
			}

			Vec2<T> operator- (const T other)
			{
				return { x - other, y - other };
			}

			Vec2<T> operator* (const T other)
			{
				return { x * other, y * other };
			}

			Vec2<T> operator/ (const T other)
			{
				return { x / other, y / other };
			}
		};
	}
}
