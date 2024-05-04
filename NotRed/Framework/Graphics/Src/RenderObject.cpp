#include "Precompiled.h"
#include "RenderObject.h"

#include "Model.h"

namespace NotRed::Graphics
{
    void RenderObject::Terminate()
    {
        meshBuffer.Terminate();
    }

    RenderGroup CreateRenderGropu(const Model& model)
    {
        RenderGroup renderGroup;

        renderGroup.resize(model.meshData.size());
        for (const Model::MeshData& meshData : model.meshData)
        {
            RenderObject& renderObject = renderGroup.emplace_back();
            renderObject.meshBuffer.Initialize(meshData.mesh);
            if (meshData.materialIndex < model.meterialData.size())
            {
                //blabla
            }
        }

        return renderGroup;
    }

    void CleanRenderGroup(RenderGroup& renderGroup)
    {
    }
}