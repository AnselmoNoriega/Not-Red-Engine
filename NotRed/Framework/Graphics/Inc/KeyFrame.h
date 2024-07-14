#pragma once

#include "Common.h"

namespace NotRed::Graphics
{
	enum class EaseType
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
	};

	template<class T>
	struct Keyframe
	{
		Keyframe() = default;
		Keyframe(const T& k, float t, EaseType e = EaseType::Linear)
			: key(k)
			, time(t)
			, easeType(e)
		{

		}
		T key = T();
		float time = 0.0f;
		EaseType easeType = EaseType::Linear;
	};

	using AnimationCallback = std::function<void()>;

	template<class T>
	using keyFrames = std::vector<Keyframe<T>>;

	using PositionKeys = keyFrames<Math::Vector3>;
	using RotationKeys = keyFrames<Math::Quaternion>;
	using ScaleKeys = keyFrames<Math::Vector3>;
	using EventKeys = keyFrames<AnimationCallback>;
}