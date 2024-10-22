#pragma once

namespace NotRed
{
    enum class ComponentId
    {
        Invalid,
        Transform,
        Camera,
        FPSCamera,
        Count
    };
    enum class ServiceId
    {
        Invalid,
        Camera,
        Count
    };
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() {return static_cast<uint32_t>(id);}\
	uint32_t GetTypeId() const override {return StaticGetTypeId(); }