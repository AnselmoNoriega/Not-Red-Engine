#pragma once

namespace NotRed::Math
{
	struct Vector2
	{
		union
		{
			struct { float x, y; };
			std::array<float, 2> v;// same as float[2]
		};

		constexpr Vector2() noexcept : Vector2(0.0f) {}
		explicit constexpr Vector2(float f) noexcept : Vector2(f, f) {}
		constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

		// commonly used vectors
		const static Vector2 Zero;
		const static Vector2 One;
		const static Vector2 XAxis;
		const static Vector2 YAxis;

		constexpr Vector2 operator-() const { return { -x, -y }; }
		constexpr Vector2 operator+(Vector2 b) const { return { x + b.x, y + b.y }; }
		constexpr Vector2 operator-(Vector2 b) const { return { x - b.x, y - b.y }; }
		constexpr Vector2 operator*(float s) const { return { x * s, y * s }; }
		constexpr Vector2 operator/(float s) const { return { x / s, y / s }; }

		constexpr Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
		constexpr Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
		constexpr Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
		constexpr Vector2& operator/=(float s) { x /= s; y /= s; return *this; }
	};
}