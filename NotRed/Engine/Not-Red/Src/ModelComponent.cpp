#include "Precompiled.h"
#include "ModelComponent.h"
#include "SaveUtil.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void ModelComponent::Initialize()
{
	ModelManager* mm = ModelManager::Get();
	mModelId = mm->GetModelID(mFileName);
	if (mm->GetModel(mModelId) == nullptr)
	{
		mModelId = mm->LoadModel(mFileName);
		for (const std::string& animation : mAnimations)
		{
			mm->AddAnimation(mModelId, animation);
		}
	}
	ASSERT(mm->GetModel(mModelId) != nullptr, "ModelComponent: model is null");
	RenderObjectComponent::Initialize();
}

void ModelComponent::Terminate()
{
	RenderObjectComponent::Terminate();
}

void ModelComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	RenderObjectComponent::Serialize(doc, value);

	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
	SaveUtil::SaveStringArray("Animations", mAnimations, doc, componentValue);

	value.AddMember("ModelComponent", componentValue, doc.GetAllocator());
}

void ModelComponent::Deserialize(const rapidjson::Value& value)
{
	RenderObjectComponent::Deserialize(value);
	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Animations"))
	{
		mAnimations.clear();
		auto animations = value["Animations"].GetArray();
		for (auto& animation : animations)
		{
			mAnimations.push_back(animation.GetString());
		}
	}
}

ModelID ModelComponent::GetModelId() const
{
	return mModelId;
}

const Model& ModelComponent::GetModel() const
{
	return *ModelManager::Get()->GetModel(mModelId);
}