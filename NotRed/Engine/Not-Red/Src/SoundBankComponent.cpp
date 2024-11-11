#include "Precompiled.h"
#include "SoundBankComponent.h"

#include "SaveUtil.h"

using namespace NotRed;
using namespace NotRed::Audio;

void SoundBankComponent::Initialize()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& data : mSoundEffects)
    {
        data.second.soundID = sm->Load(data.second.fileName);
    }
}

void SoundBankComponent::Terminate()
{
    StopAll();
}

void SoundBankComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    if (!mSoundEffects.empty())
    {
        rapidjson::Value soundEffectsValue(rapidjson::kObjectType);
        for (auto& effect : mSoundEffects)
        {
            rapidjson::Value effectValue(rapidjson::kObjectType);
            SaveUtil::SaveString("FileName", effect.second.fileName.c_str(), doc, effectValue);
            SaveUtil::SaveBool("Looping", effect.second.looping, doc, effectValue);

            rapidjson::GenericStringRef<char> str(effect.first.c_str());
            soundEffectsValue.AddMember(str, effectValue, doc.GetAllocator());
        }
        soundEffectsValue.AddMember("SoundEffects", soundEffectsValue, doc.GetAllocator());
    }
    value.AddMember("SoundBankComponent", componentValue, doc.GetAllocator());
}

void SoundBankComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("SoundEffects"))
    {
        auto soundEffects = value["SoundEffects"].GetObj();
        for (auto& effect : soundEffects)
        {
            SoundEffectData& data = mSoundEffects[effect.name.GetString()];
            if (effect.value.HasMember("FileName"))
            {
                data.fileName = effect.value["FileName"].GetString();
            }
            if (effect.value.HasMember("Looping"))
            {
                data.looping = effect.value["Looping"].GetBool();
            }
        }
    }
}

void SoundBankComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("SoundEffects"))
    {
        ImGui::Indent();
        for (auto& effect : mSoundEffects)
        {
            ImGui::Text(effect.first.c_str());
            std::string buttonLabel = "Play##" + effect.first;
            if (ImGui::Button(buttonLabel.c_str()))
            {
                Play(effect.first);
            }
            ImGui::SameLine();
            buttonLabel = "Stop##" + effect.first;
            if (ImGui::Button(buttonLabel.c_str()))
            {
                Stop(effect.first);
            }
        }
        ImGui::Unindent();
    }
}

void SoundBankComponent::Play(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
    {
        SoundEffectManager::Get()->Play(iter->second.soundID);
    }
}

void SoundBankComponent::Stop(const std::string& key)
{
    auto iter = mSoundEffects.find(key);
    if (iter != mSoundEffects.end())
    {
        SoundEffectManager::Get()->Stop(iter->second.soundID);
    }
}

void SoundBankComponent::StopAll()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    for (auto& effect : mSoundEffects)
    {
        sm->Stop(effect.second.soundID);
    }
}
