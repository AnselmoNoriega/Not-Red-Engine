#include "Precompiled.h"
#include "SoundEffectComponent.h"

#include "SaveUtil.h"

using namespace NotRed;
using namespace NotRed::Audio;

void SoundEffectComponent::Initialize()
{
    ASSERT(!mFileName.empty(), "SoundEffectComponent: no file Loded!");
    mSoundID = SoundEffectManager::Get()->Load(mFileName);
}

void SoundEffectComponent::Terminate()
{
    Stop();
}

void SoundEffectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
    SaveUtil::SaveBool("Looping", mLooping, doc, componentValue);
    value.AddMember("SoundEffectComponent", componentValue, doc.GetAllocator());
}

void SoundEffectComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("FileName"))
    {
        mFileName = value["FileName"].GetString();
    }
    if (value.HasMember("Looping"))
    {
        mLooping = value["Looping"].GetBool();
    }
}

void SoundEffectComponent::DebugUI()
{
    ImGui::Text(mFileName.c_str());
    if(ImGui::Button("Play"))
    {
        Play();
    }
    if(ImGui::Button("Stop"))
    {
        Stop();
    }
}

void SoundEffectComponent::Play()
{
    SoundEffectManager::Get()->Play(mSoundID);
}

void SoundEffectComponent::Stop()
{
    SoundEffectManager::Get()->Stop(mSoundID);
}
