#pragma once

#include "MeshBuffer.h"
#include "TextureManager.h"
#include "Transform.h"

namespace NotRed::Graphics
{
    struct RenderObject
    {
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        TextureID diffuseMapID;
        TextureID normalMapID;
        TextureID specMapID;
    };
}
