#include "Precompiled.h"
#include "GameObject.h"

using namespace NotRed;

static uint32_t gUniqueId = 0;

void GameObject::Initialize()
{
	ASSERT(!mInitialized, " GameObject: is alreadt initailized");
	for (auto& component : mComponents)
	{
		component->Initialize();
	}
	mUniqueId = ++gUniqueId;
	mInitialized = true;
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
		component.reset();
	}
}

void NotRed::GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void GameObject::DebugUI()
{
	ImGui::PushID(mUniqueId);
	if (ImGui::CollapsingHeader(mName.c_str()))
	{
		for (auto& component : mComponents)
		{
			component->DebugUI();
		}
	}

	if (!mTemplateFilePath.empty())
	{
		if (ImGui::Button("Save"))
		{
			Save();
		}
	}

	ImGui::PopID();
	ImGui::Separator();
}

void GameObject::Save()
{
	if (mTemplateFilePath.empty())
	{
		return;
	}

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Value components(rapidjson::kObjectType);

	for (auto& component : mComponents)
	{
		component->Serialize(doc, components);
	}

	doc.AddMember("Components", components, doc.GetAllocator());

	// parented objects
	FILE* file = nullptr;
	auto err = fopen_s(&file, mTemplateFilePath.string().c_str(), "w");

	ASSERT(err == 0, "GameObject: failed to open template file %s", mTemplateFilePath.string().c_str());
	
	char writeBuffer[65536];
	rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	doc.Accept(writer);

	fclose(file);
}

void GameObject::SetName(std::string& name)
{
	mName = std::move(name);
}

const std::string GameObject::GetName() const
{
	return mName;
}

uint32_t GameObject::GetUniqueId() const
{
	return mUniqueId;
}