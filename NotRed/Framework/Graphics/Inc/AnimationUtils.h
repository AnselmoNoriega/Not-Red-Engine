#pragma once

#include "ModelManager.h";
#include "Animator.h"

namespace NotRed::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<Math::Matrix4>;

	void ComputeBoneTransforms(ModelId id, BoneTransforms& boneTransforms, const Animator* animator = nullptr);
	void ApplyBoneOfset(ModelId id, BoneTransforms& boneTransforms);
	void DrawSkeleton(ModelId id, BoneTransforms& boneTransforms);
}