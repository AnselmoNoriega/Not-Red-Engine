#include "Precompiled.h"
#include "Animation.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time, EaseType easeType)
	{
		float t = (time - startTime) / (endTime - startTime);
		switch (easeType)
		{
		case EaseType::Linear:break;
		case EaseType::EaseInQuad:t = t * t; break;
		case EaseType::EaseOutQuad:t = t * (2.0f - t); break;
		case EaseType::EaseInOutQuad:
		{
			t = t * 2.0f;
			if (t < 1.0f)
			{
				t = 0.5f * t * t;
			}
			else
			{
				t = t - 1.0f;
				t = -0.5f * ((t * (t - 2.0f)) - 1.0f);
			}
		}
		default:
			break;
		}
		return t;
	}
}

Transform Animation::GetTransform(float time) const
{
	Transform transform;
	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);

	return transform;
}

float Animation::GetDuration() const
{
	return mDuration;
}

void Animation::PlayEvents(float prevTime, float currentTime)
{
	for (uint32_t i = 0; i < mEventKeys.size(); ++i)
	{
		if (mEventKeys[i].time > prevTime && mEventKeys[i].time <= currentTime)
		{
			mEventKeys[i].key();
		}
	}
}

Math::Vector3 Animation::GetPosition(float time) const
{
	for (uint32_t i = 0; i < mPositionKeys.size(); ++i)
	{
		if (time < mPositionKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time, mPositionKeys[i].easeType);
				return Math::Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, t);
			}
			return mPositionKeys[i].key;
		}
	}

	if (!mPositionKeys.empty())
	{
		return mPositionKeys.back().key;
	}
	return Math::Vector3::Zero;
}

Math::Quaternion Animation::GetRotation(float time) const
{
	for (uint32_t i = 0; i < mRotationKeys.size(); ++i)
	{
		if (time < mRotationKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
				return Math::Quaternion::slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
			}
			return mRotationKeys[i].key;
		}
	}

	if (!mRotationKeys.empty())
	{
		return mRotationKeys.back().key;
	}
	return Math::Quaternion::Identity;
}

Math::Vector3 Animation::GetScale(float time) const
{
	for (uint32_t i = 0; i < mScaleKeys.size(); ++i)
	{
		if (time < mScaleKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time, mScaleKeys[i].easeType);
				return Math::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
			}
			return mScaleKeys[i].key;
		}
	}

	if (!mScaleKeys.empty())
	{
		return mScaleKeys.back().key;
	}
	return Math::Vector3::One;
}