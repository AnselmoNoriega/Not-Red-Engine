#pragma once

#include "Animation.h"

namespace NotRed::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const Math::Vector3& pos, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rot, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time, EaseType easeType = EaseType::Linear);

		[[nodiscard]] Animation  Build();
	private:
		Animation mWorkingCopy;
	};
}