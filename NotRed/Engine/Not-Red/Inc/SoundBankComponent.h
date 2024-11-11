#pragma once

#include "Component.h"

namespace NotRed
{
    class SoundBankComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::SoundBank);

        void Initialize() override;
        void Terminate() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        void Deserialize(const rapidjson::Value& value) override;

        void DebugUI() override;

        void Play(const std::string& key);
        void Stop(const std::string& key);
        void StopAll();

    private:
        struct SoundEffectData
        {
            std::string fileName;
            bool looping = false;
            Audio::SoundID soundID;
        };

        using SoundEffects = std::unordered_map<std::string, SoundEffectData>;

    private:
        SoundEffects mSoundEffects;
    };
}