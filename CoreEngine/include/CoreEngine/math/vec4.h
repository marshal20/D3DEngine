#pragma once

namespace ce
{
	namespace math
	{
		template<typename T>
		struct Vec4
		{
			T x;
			T y;
			T z;
			T w;

			Vec4()
			{
				x = static_cast<T>(0);
				y = static_cast<T>(0);
				z = static_cast<T>(0);
				w = static_cast<T>(0);
			}

			Vec4(const T x_val, const T y_val, const T z_val, const T w_val)
			{
				x = x_val;
				y = y_val;
				z = z_val;
				w = w_val;
			}

			Vec4(const Vec4<T>& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}

			Vec4<T>& operator= (const Vec4<T>& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
				return *this;
			}

			Vec4<T> operator+ (const Vec4<T>& other)
			{
				return { x + other.x, y + other.y, z + other.z, w + other.w };
			}

			Vec4<T> operator- (const Vec4<T>& other)
			{
				return { x - other.x, y - other.y, z - other.z, w - other.w };
			}

			Vec4<T> operator* (const Vec4<T>& other)
			{
				return { x * other.x, y * other.y, z * other.z, w * other.w };
			}

			Vec4<T> operator/ (const Vec4<T>& other)
			{
				return { x / other.x, y / other.y, z / other.z, w / other.w };
			}

			Vec4<T> operator+ (const T other)
			{
				return { x + other, y + other, z + other, w + other };
			}

			Vec4<T> operator- (const T other)
			{
				return { x - other, y - other, z - other, w - other };
			}

			Vec4<T> operator* (const T other)
			{
				return { x * other, y * other, z * other, w * other };
			}

			Vec4<T> operator/ (const T other)
			{
				return { x / other, y / other, z / other, w / other };
			}

			bool operator== (const Vec4<T>& other) const
			{
				return (x == other.x && y == other.y && z == other.z && w == other.w);
			}

			bool operator!= (const Vec4<T>& other) const
			{
				return (x != other.x || y != other.y || z != other.z || w != other.w);
			}
		};
	}
}
