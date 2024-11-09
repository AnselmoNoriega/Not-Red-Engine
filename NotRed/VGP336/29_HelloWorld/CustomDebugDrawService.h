#pragma once

#include "CustomTypeIDs.h"

class CustomDebugDrawComponent;

class CustomDebugDrawService : public NotRed::Service
{
public:
    SET_TYPE_ID(CustomServiceID::CustomDebugDrawDisplay);

    void Render() override;

    void Register(CustomDebugDrawComponent* debugDrawComponent);
    void Unregister(CustomDebugDrawComponent* debugDrawComponent);

private:
    using CustomDebugDrawComponents = std::vector<CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};