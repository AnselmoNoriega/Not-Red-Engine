#include "Precompiled.h"
#include "RenderObjectComponent.h"

#include "RenderService.h"
#include "GameWorld.h"

using namespace NotRed;

void RenderObjectComponent::Initialize()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Register(this);
}

void RenderObjectComponent::Terminate()
{
	RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}