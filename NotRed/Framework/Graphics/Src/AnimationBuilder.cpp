#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
	template<class T>
	inline void PushKey(keyFrames<T>& keyframes, const T& value, float t, EaseType easetype)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "Animation: ");
		keyframes.emplace_back(value, t, easetype);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& pos, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionKeys, pos, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& rot, float time, EaseType easeType)
{

	PushKey(mWorkingCopy.mRotationKeys, rot, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time, EaseType easeType)
{

	PushKey(mWorkingCopy.mScaleKeys, scale, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty() ||
		!mWorkingCopy.mRotationKeys.empty() ||
		!mWorkingCopy.mScaleKeys.empty(),
		"AnimationBuilder: no animaiton keys are present");
	return mWorkingCopy;
}