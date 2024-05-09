#include "Precompiled.h"
#include "RenderObject.h"

#include "Model.h"

namespace NotRed::Graphics
{
    void RenderObject::Terminate()
    {
        meshBuffer.Terminate();
    }

    RenderGroup Graphics::CreateRenderGroup(const Model& model)
    {
        auto TryLoadTexture = [](const auto& textureName)->TextureID
            {
                if (textureName.empty())
                {
                    return 0;
                }
                return TextureManager::Get()->LoadTexture(textureName, false);
            };

        RenderGroup renderGroup;
        renderGroup.resize(model.meshData.size());

        for (const Model::MeshData& meshData : model.meshData)
        {
            RenderObject& renderObject = renderGroup.emplace_back();
            renderObject.meshBuffer.Initialize(meshData.mesh);
            if (meshData.materialIndex < model.meterialData.size())
            {
                const Model::MeterialData& materialData = model.meterialData[meshData.materialIndex];
                renderObject.material = materialData.material;
                renderObject.diffuseMapID = TryLoadTexture(materialData.diffuseMapName);
                renderObject.normalMapID = TryLoadTexture(materialData.normalMapName);
                renderObject.bumpMapID = TryLoadTexture(materialData.bumpMapName);
                renderObject.specMapID = TryLoadTexture(materialData.specularMapName);
            }
        }

        return renderGroup;
    }

    void CleanRenderGroup(RenderGroup& renderGroup)
    {
        for (RenderObject& renderObject : renderGroup)
        {
            renderObject.Terminate();
        }
    }
}