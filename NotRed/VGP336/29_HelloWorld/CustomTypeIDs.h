#pragma once

#include <Not-Red/Inc/NotRed.h>

enum class CustomComponentID
{
    CustomDebugDraw = static_cast<uint32_t>(NotRed::ComponentId::Count)
};
enum class CustomServiceID
{
    CustomDebugDrawDisplay = static_cast<uint32_t>(NotRed::ServiceId::Count)
};