#include "Precompiled.h"
#include "AnimationUtils.h"
#include "SimpleDraw.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
	void ComputeBoneTransformRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone == nullptr)
		{
			return;
		}
		if (animator != nullptr)
		{
			boneTransforms[bone->index] = animator->GetToParentTransform(bone);
		}
		else
		{
			boneTransforms[bone->index] = bone->toParentTransform;
		}
		if (bone->parent != nullptr)
		{
			boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
		}
		for (const Bone* child : bone->children)
		{
			ComputeBoneTransformRecursive(child, boneTransforms, animator);
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms, const Animator* animator)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);
		ComputeBoneTransformRecursive(model->skeleton->root, boneTransforms, animator);
	}

}

void AnimationUtil::ApplyBoneOfset(ModelID id, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}

void AnimationUtil::DrawSkeleton(ModelID id, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	if (model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const Math::Vector3 posA = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 posB = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(posA, posB, Colors::Blue);
				SimpleDraw::AddSphere(4, 4, 0.03f, posA, Colors::Pink);
			}
		}
	}
}