#include "Precompiled.h"
#include "Animator.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void Animator::Initialize(ModelID id)
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;
}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mAnimationTick > animClip.ticksDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick > animClip.ticksDuration)
			{
				mAnimationTick -= animClip.ticksDuration;
			}
		}
		else
		{
			mAnimationTick = animClip.ticksDuration;
		}
	}
}

bool Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.ticksDuration;
}

size_t Graphics::Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

Math::Matrix4 Graphics::Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimation[bone->index].get();
	if (animation == nullptr)
	{
		return Math::Matrix4::Identity;
	}
	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix();
}