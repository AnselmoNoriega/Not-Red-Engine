#pragma once

#include "Service.h"

namespace NotRed
{
    class RigidBodyComponent;

    class PhysicsService : public Service
    {
    public:
        SET_TYPE_ID(ServiceId::Physics);

        void Update(float dt) override;
        void DebugUI() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        void Deserialize(const rapidjson::Value& value) override;

        void Register(RigidBodyComponent* rigidBodyComponent);
        void Unregister(RigidBodyComponent* rigidBodyComponent);

        void SetEnabled(bool enabled);

    private:
        bool mEnabled = true;
    };
}