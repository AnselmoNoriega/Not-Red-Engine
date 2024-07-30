#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void Graphics::RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup Graphics::CreateRenderGroup(ModelID id)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	return CreateRenderGroup(*model, id);
}

RenderGroup Graphics::CreateRenderGroup(const Model& model, ModelID id)
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

		renderObject.modelID = id;
		renderObject.skeleton = model.skeleton.get();
	}
	return renderGroup;
}

void Graphics::CleanRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}

void Graphics::SetRenderGroupPosition(RenderGroup& renderGroup, const Math::Vector3& position)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.transform.position = position;
	}
}
