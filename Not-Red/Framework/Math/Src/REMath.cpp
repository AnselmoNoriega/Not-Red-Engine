#include "Precompiled.h"
#include "REMath.h"

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