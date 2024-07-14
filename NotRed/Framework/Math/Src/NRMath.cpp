#include "Precompiled.h"
#include "NRMath.h"

using namespace NotRed::Math;

const Vector2 Vector2::Zero(0.0f);
const Vector2 Vector2::One(1.0f, 1.0f);
const Vector2 Vector2::XAxis(1.0f, 0.0f);
const Vector2 Vector2::YAxis(0.0f, 1.0f);

const Vector3 Vector3::Zero(0.0f);
const Vector3 Vector3::One(1.0f);
const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);

const Matrix4 Matrix4::Zero({ 0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0 });

const Matrix4 Matrix4::Identity({ 1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1 });

const Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };
const Quaternion Quaternion::Zero = { 0.0f, 0.0f, 0.0f, 0.0f };

void Quaternion::Conjugate() noexcept
{
	x = -x; y = -y; z = -z;
}

void Quaternion::Invers()noexcept
{
	Conjugate();
	*this = *this / MagnitudesSquared();
}
float Quaternion::Magnitude() const noexcept
{
	return sqrt(MagnitudesSquared());
}

float Quaternion::MagnitudesSquared() const noexcept
{
	return (x * x + y * y + z * z + w * w);
}

void Quaternion::Normalize() noexcept
{
	*this = *this / Magnitude();
}

float Quaternion::Dot(const Quaternion& q) const noexcept
{
	return x * q.x + y * q.y + z * q.z + w * q.w;
}

//static functions
Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	return { -q.x,-q.y,-q.z,-q.w };
}
float Quaternion::Magnitude(const Quaternion& q)
{
	return sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}
Quaternion Quaternion::Normalize(const Quaternion& q)
{
	return q / Quaternion::Magnitude(q);
}

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle) noexcept
{
	const float c = cos(angle * 0.5f);
	const float s = sin(angle * 0.5f);
	const Vector3 a = Math::Normalize(axis);
	return { a.x * s, a.y * s, a.z * s, c };
}
Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
	const float cy = cos(yaw * 0.5f);
	const float sy = sin(yaw * 0.5f);

	const float cp = cos(pitch * 0.5f);
	const float sp = sin(pitch * 0.5f);


	const float cr = cos(roll * 0.5f);
	const float sr = sin(roll * 0.5f);

	return
	{
	sr * cp * cy - cr * sp * sy,
	cr * sp * cy + sr * cp * sy,
	cr * cp * sy - sr * sp * cy,
	cr * cp * cy + sr * sp * sy
	};
}
Quaternion Quaternion::CreateFromRotationMatrix(const Matrix4& m) noexcept
{
	const float w = sqrt(m._11 + m._22 + m._33 + 1) * 0.5f;
	const float x = sqrt(m._11 - m._22 - m._33 + 1) * 0.5f;
	const float y = sqrt(-m._11 + m._22 - m._33 + 1) * 0.5f;
	const float z = sqrt(-m._11 - m._22 + m._33 + 1) * 0.5f;

	Quaternion q;
	if (w >= x && w >= y && w >= z)
	{
		q.w = w;
		q.x = (m._23 - m._32) / (4.0f * w);
		q.y = (m._31 - m._13) / (4.0f * w);
		q.z = (m._12 - m._21) / (4.0f * w);
	}
	else if (x >= w && x >= y && x >= z)
	{
		q.w = (m._23 - m._32) / (4.0f * x);
		q.x = x;
		q.y = (m._12 - m._21) / (4.0f * x);
		q.z = (m._31 - m._13) / (4.0f * x);
	}
	else if (y >= w && y >= x && y >= z)
	{
		q.w = (m._31 - m._13) / (4.0f * y);
		q.x = (m._12 - m._21) / (4.0f * y);
		q.y = y;
		q.z = (m._23 - m._32) / (4.0f * y);
	}
	else if (y >= w && y >= x && y >= z)
	{
		q.w = (m._12 - m._21) / (4.0f * z);
		q.x = (m._31 - m._13) / (4.0f * z);
		q.y = (m._23 - m._32) / (4.0f * z);
		q.z = z;
	}
	return q;
}

Quaternion Quaternion::Lerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	return q0 * (1.0f - t) + (q1 * t);
}
Quaternion Quaternion::slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	float dot = q0.Dot(q1);
	float q1Scale = 1.0f;

	if (dot < 0.0f)
	{
		dot = -dot;
		q1Scale = -1.0f;
	}

	if (dot > 0.9999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	const float theta = acosf(dot);
	const float sinTheta = sinf(theta);
	const float scale0 = sinf(theta * (1.0f - t)) / sinTheta;
	const float scale1 = q1Scale * sinf(theta * t) / sinTheta;

	Quaternion q =
	{
		q0.x * scale0 + q1.x * scale1,
		q0.y * scale0 + q1.y * scale1,
		q0.z * scale0 + q1.z * scale1,
		q0.w * scale0 + q1.w * scale1
	};

	return Normalize(q);
}