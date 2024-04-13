#pragma once

#include "MeshBuffer.h"
#include "Texture.h"
#include "Transform.h"

namespace NotRed::Graphics
{
    struct RenderObject
    {
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;
        Texture texture;
    };
}
