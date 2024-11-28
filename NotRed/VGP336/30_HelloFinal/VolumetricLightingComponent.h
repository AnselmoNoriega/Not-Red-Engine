#pragma once
/*
#include "CustomTypeIDs.h"

class VolumetricLightingComponent : public NotRed::Component
{
public: 
    SET_TYPE_ID(CustomComponentID::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void Update(float dt) override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const NotRed::TransformComponent* mTransformComponent = nullptr;
    NotRed::Math::Vector3 mPosition{};
    NotRed::Color mColor = NotRed::Colors::Wheat;
    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0.0f;
};*/