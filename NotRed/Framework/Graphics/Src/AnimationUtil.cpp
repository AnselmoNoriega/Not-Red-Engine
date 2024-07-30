#include "Precompiled.h"
#include "AnimationUtils.h"
#include "SimpleDraw.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
    void ComputeBoneTransformRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms)
    {
        if (bone == nullptr)
        {
            return;
        }

        boneTransforms[bone->index] = bone->toParentTransform;
        if (bone->parent != nullptr)
        {
            boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
        }

        for (const Bone* child : bone->children)
        {
            ComputeBoneTransformRecursive(child, boneTransforms);
        }
    }
}
void AnimationUtil::ComputeBoneTransforms(ModelID id, BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(id);
    if (model->skeleton != nullptr)
    {
        boneTransforms.resize(model->skeleton->bones.size(), Math::Matrix4::Identity);
        ComputeBoneTransformRecursive(model->skeleton->root, boneTransforms);
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
                if (Math::DistanceSqr(posA, posB) > 0.0001f)
                {
                    SimpleDraw::AddLine(posA, posB, Colors::Blue);
                    SimpleDraw::AddSphere(10, 10, 0.03f, posA, Colors::Pink);
                }
            }
        }
    }
}