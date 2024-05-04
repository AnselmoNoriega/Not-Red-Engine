#pragma once

#include "MeshBuffer.h"
#include "Material.h"
#include "TextureManager.h"
#include "Transform.h"

namespace NotRed::Graphics
{
    struct Model;

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

    using RenderGroup = std::vector<RenderObject>;
    [[nodiscard]] RenderGroup CreateRenderGropu(const Model& model);
    void CleanRenderGroup(RenderGroup& renderGroup);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
    {
        for (const RenderObject& renderObject : renderGroup)
        {
            effect.Render(renderObject);
        }
    }
}
