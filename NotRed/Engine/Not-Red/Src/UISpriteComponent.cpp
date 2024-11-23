#include "Precompiled.h"
#include "UISpriteComponent.h"

#include "GameWorld.h"
#include "UIRenderService.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void UISpriteComponent::Initialize()
{
	ASSERT(!mTexturePath.empty(), "UISpriteComponent: texture path is not set");
	mUISprite.Initialize(mTexturePath);
	UIRenderService* renderService = GetOwner().GetWorld().GetService<UIRenderService>();
	renderService->Register(this);
}

void UISpriteComponent::Terminate()
{
	UIRenderService* renderService = GetOwner().GetWorld().GetService<UIRenderService>();
	renderService->Unregister(this);
	mUISprite.Terminate();
}

void UISpriteComponent::Render()
{
	UISpriteRenderer::Get()->Render(&mUISprite);
}

void UISpriteComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("Texture"))
	{
		mTexturePath = value["Texture"].GetString();
	}
	if (value.HasMember("Position"))
	{
		auto pos = value["Position"].GetArray();
		const float x = pos[0].GetFloat();
		const float y = pos[1].GetFloat();
		mUISprite.SetPosition({ x, y });
	}
	if (value.HasMember("Scale"))
	{
		auto scale = value["Scale"].GetArray();
		const float x = scale[0].GetFloat();
		const float y = scale[1].GetFloat();
		mUISprite.SetScale({ x, y });
	}
	if (value.HasMember("Color"))
	{
		auto color = value["Color"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();
		mUISprite.SetColor({ r,g,b,a });
	}
	if (value.HasMember("Rotation"))
	{
		const float rotation = value["Rotation"].GetFloat();
		mUISprite.SetRotation(rotation);
	}
	if (value.HasMember("Pivot"))
	{
		std::string pivot = value["Pivot"].GetString();
		if (pivot == "TopLeft")
		{
			mUISprite.SetPivot(Pivot::TopLeft);
		}
		else if (pivot == "Top")
		{
			mUISprite.SetPivot(Pivot::Top);
		}
		else if (pivot == "TopRight")
		{
			mUISprite.SetPivot(Pivot::TopRight);
		}
		else if (pivot == "Left")
		{
			mUISprite.SetPivot(Pivot::Left);
		}
		else if (pivot == "Center")
		{
			mUISprite.SetPivot(Pivot::Center);
		}
		else if (pivot == "Right")
		{
			mUISprite.SetPivot(Pivot::Right);
		}
		else if (pivot == "BottomLeft")
		{
			mUISprite.SetPivot(Pivot::BottomLeft);
		}
		else if (pivot == "Bottom")
		{
			mUISprite.SetPivot(Pivot::Bottom);
		}
		else if (pivot == "BottomRight")
		{
			mUISprite.SetPivot(Pivot::BottomRight);
		}
		else
		{
			ASSERT(false, "UISpriteComponent: invalided pivot %s", pivot.c_str());
		}
	}
	if (value.HasMember("Flip"))
	{
		std::string flip = value["Flip"].GetString();
		if (flip == "None")
		{
			mUISprite.SetFlip(Flip::None);
		}
		else if (flip == "Horizontal")
		{
			mUISprite.SetFlip(Flip::Horizontal);
		}
		else if (flip == "Vertical")
		{
			mUISprite.SetFlip(Flip::Vertical);
		}
		else if (flip == "Both")
		{
			mUISprite.SetFlip(Flip::Both);
		}
		else
		{
			ASSERT(false, "UISpriteComponent: invalid flip %s", flip.c_str());
		}
	}
}