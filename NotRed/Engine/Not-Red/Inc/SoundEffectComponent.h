#pragma once

#include "Component.h"

namespace NotRed
{
    class SoundEffectComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::SoundEffect);

        void Initialize() override;
        void Terminate() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        void Deserialize(const rapidjson::Value& value) override;

        void DebugUI() override;

        void Play();
        void Stop();

    private:
        std::string mFileName;
        bool mLooping = false;
        Audio::SoundID mSoundID;
    };
}