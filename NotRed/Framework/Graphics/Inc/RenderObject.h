#pragma once

#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"

namespace NotRed::Graphics
{
    struct RenderObject
    {
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;

        Material material;

        TextureID diffuseMapID;
        TextureID normalMapID;
        TextureID specMapID;
        TextureID bumpMapID;
    };
}
