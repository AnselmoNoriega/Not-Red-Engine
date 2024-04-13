#pragma once

namespace NotRed::Math
{
	struct Vector3
	{
		union
		{
			struct { float x, y, z; };
			std::array<float, 3> v;// same as float[3]
		};

		constexpr Vector3() noexcept : Vector3(0.0f) {}
		explicit constexpr Vector3(float f) noexcept : Vector3(f, f, f) {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

		// commonly used vectors
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator-() const { return { -x, -y, -z }; }
		constexpr Vector3 operator+(Vector3 b) const { return { x + b.x, y + b.y, z + b.z }; }
		constexpr Vector3 operator-(Vector3 b) const { return { x - b.x, y - b.y, z - b.z }; }
		constexpr Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
		constexpr Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }

		constexpr Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		constexpr Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		constexpr Vector3& operator*=(float s) { x *= s; y *= s, z *= s; return *this; }
		constexpr Vector3& operator/=(float s) { x /= s; y /= s, z /= s; return *this; }
	};
}