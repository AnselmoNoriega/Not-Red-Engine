#pragma once

namespace NotRed::Math
{
	struct Matrix4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};

		constexpr Matrix4() noexcept
			: Matrix4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			)
		{

		}

		constexpr Matrix4(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44
		) noexcept
			: _11(_11), _12(_12), _13(_13), _14(_14)
			, _21(_21), _22(_22), _23(_23), _24(_24)
			, _31(_31), _32(_32), _33(_33), _34(_34)
			, _41(_41), _42(_42), _43(_43), _44(_44)
		{}

		const static Matrix4 Zero;
		const static Matrix4 Identity;

		static float Determinant3x3(
			float a1, float a2, float a3,
			float b1, float b2, float b3,
			float c1, float c2, float c3)
		{
			return a1 * (b2 * c3 - b3 * c2) -
				a2 * (b1 * c3 - b3 * c1) +
				a3 * (b1 * c2 - b2 * c1);
		}

		float Determinant() const
		{
			float det = 0.0f;

			det += _11 * Determinant3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44);
			det -= _12 * Determinant3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44);
			det += _13 * Determinant3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44);
			det -= _14 * Determinant3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43);

			return det;
		}

		Matrix4 Inverse() const
		{
			float det = Determinant();
			if (std::fabs(det) < 1e-6f) {
				return *this;
			}

			float invDet = 1.0f / det;

			Matrix4 result;

			// Compute adjugate matrix and scale by 1/determinant
			result._11 = Determinant3x3(_22, _23, _24, _32, _33, _34, _42, _43, _44) * invDet;
			result._12 = -Determinant3x3(_12, _13, _14, _32, _33, _34, _42, _43, _44) * invDet;
			result._13 = Determinant3x3(_12, _13, _14, _22, _23, _24, _42, _43, _44) * invDet;
			result._14 = -Determinant3x3(_12, _13, _14, _22, _23, _24, _32, _33, _34) * invDet;

			result._21 = -Determinant3x3(_21, _23, _24, _31, _33, _34, _41, _43, _44) * invDet;
			result._22 = Determinant3x3(_11, _13, _14, _31, _33, _34, _41, _43, _44) * invDet;
			result._23 = -Determinant3x3(_11, _13, _14, _21, _23, _24, _41, _43, _44) * invDet;
			result._24 = Determinant3x3(_11, _13, _14, _21, _23, _24, _31, _33, _34) * invDet;

			result._31 = Determinant3x3(_21, _22, _24, _31, _32, _34, _41, _42, _44) * invDet;
			result._32 = -Determinant3x3(_11, _12, _14, _31, _32, _34, _41, _42, _44) * invDet;
			result._33 = Determinant3x3(_11, _12, _14, _21, _22, _24, _41, _42, _44) * invDet;
			result._34 = -Determinant3x3(_11, _12, _14, _21, _22, _24, _31, _32, _34) * invDet;

			result._41 = -Determinant3x3(_21, _22, _23, _31, _32, _33, _41, _42, _43) * invDet;
			result._42 = Determinant3x3(_11, _12, _13, _31, _32, _33, _41, _42, _43) * invDet;
			result._43 = -Determinant3x3(_11, _12, _13, _21, _22, _23, _41, _42, _43) * invDet;
			result._44 = Determinant3x3(_11, _12, _13, _21, _22, _23, _31, _32, _33) * invDet;

			return result;
		}

		static Matrix4 Translation(float x, float y, float z)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				x, y, z, 1.0f
			);
		}

		static Matrix4 Translation(const Vector3& v)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				v.x, v.y, v.z, 1.0f
			);
		}

		static Matrix4 RotationAxis(const Vector3& axis, float rad);

		static Matrix4 RotationX(float rad)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cosf(rad), sinf(rad), 0.0f,
				0.0f, -sinf(rad), cosf(rad), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 RotationY(float rad)
		{
			return Matrix4
			(
				cosf(rad), 0.0f, -sinf(rad), 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sinf(rad), 0.0f, cosf(rad), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 RotationZ(float rad)
		{
			return Matrix4
			(
				cosf(rad), sinf(rad), 0.0f, 0.0f,
				-sinf(rad), cosf(rad), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 MatrixRotationQuaternion(const Math::Quaternion& q)
		{
			return Matrix4
			(
				1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
				(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
				(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
				0.0f,

				(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
				1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
				(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
				0.0f,

				(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
				(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
				1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
				0.0f,

				0.0f,
				0.0f,
				0.0f,
				1.0f
			);
		}

		static Matrix4 Scaling(float s)
		{
			return Matrix4
			(
				s, 0.0f, 0.0f, 0.0f,
				0.0f, s, 0.0f, 0.0f,
				0.0f, 0.0f, s, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 Scaling(float sx, float sy, float sz)
		{
			return Matrix4
			(
				sx, 0.0f, 0.0f, 0.0f,
				0.0f, sy, 0.0f, 0.0f,
				0.0f, 0.0f, sz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 Scaling(const Vector3& s)
		{
			return Matrix4
			(
				s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		constexpr Matrix4 operator-() const
		{
			return Matrix4(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44);
		}
		constexpr Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
		}
		constexpr Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
		}
		constexpr Matrix4 operator*(const Matrix4& rhs) const
		{
			return Matrix4(
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
				(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
				(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
				(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

				(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
				(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
				(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
				(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44));
		}
		constexpr Matrix4 operator*(float s) const
		{
			return Matrix4(
				_11 * s, _12 * s, _13 * s, _14 * s,
				_21 * s, _22 * s, _23 * s, _24 * s,
				_31 * s, _32 * s, _33 * s, _34 * s,
				_41 * s, _42 * s, _43 * s, _44 * s);
		}
		constexpr Matrix4 operator/(float s) const
		{
			return Matrix4(
				_11 / s, _12 / s, _13 / s, _14 / s,
				_21 / s, _22 / s, _23 / s, _24 / s,
				_31 / s, _32 / s, _33 / s, _34 / s,
				_41 / s, _42 / s, _43 / s, _44 / s);
		}
		constexpr Matrix4 operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}
	};
}