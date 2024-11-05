#include "Precompiled.h"
#include "MeshComponent.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
    RenderObjectComponent::Deserialize(value);

    ASSERT(value.HasMember("Shape") || !mModel.meshData.empty(), "MeshComponent: either needs shape data or have data already");
    Model::MeshData& meshData = (value.HasMember("Shape")) ? mModel.meshData.emplace_back() : mModel.meshData.back();
    Model::MeterialData& matData = (value.HasMember("Shape")) ? mModel.meterialData.emplace_back() : mModel.meterialData.back();

    if (value.HasMember("Shape"))
    {
        const auto& shapeData = value["Shape"].GetObj();
        if (shapeData.HasMember("Type"))
        {
            std::string shapeType = shapeData["Type"].GetString();
            if (shapeType == "Sphere")
            {
                uint32_t slices = static_cast<uint32_t>(shapeData["Slices"].GetInt());
                uint32_t rings = static_cast<uint32_t>(shapeData["Rings"].GetInt());
                float radius = shapeData["Radius"].GetFloat();
                meshData.mesh = MeshBuilder::CreateSphere(slices, rings, radius);
            }
            else if (shapeType == "Plane")
            {
                std::string direction = shapeData["Direction"].GetString();
                uint32_t rows = static_cast<uint32_t>(shapeData["Rows"].GetInt());
                uint32_t columns = static_cast<uint32_t>(shapeData["Columns"].GetInt());
                float spacing = shapeData["Spacing"].GetFloat();
                if (direction == "Vertical")
                {
                    meshData.mesh = MeshBuilder::CreateVerticalPlane(rows, columns, spacing);
                }
                else
                {
                    meshData.mesh = MeshBuilder::CreateHorizontalPlane(rows, columns, spacing);
                }
            }
            else if (shapeType == "Cube")
            {
                float size = shapeData["Size"].GetFloat();
                //meshData.mesh = MeshBuilder::CreateCubePC(rows, columns, spacing);
            }
            else
            {
                ASSERT(false, "MeshComponent: unrecognized shape type %s", shapeType.c_str());
            }

        }
    }

    if (value.HasMember("Material"))
    {
        const auto& materialData = value["Material"].GetObj();
        if (materialData.HasMember("Ambient"))
        {
            const auto& color = materialData["Ambient"].GetArray();
            matData.material.ambient.r = color[0].GetFloat();
            matData.material.ambient.g = color[1].GetFloat();
            matData.material.ambient.b = color[2].GetFloat();
            matData.material.ambient.a = color[3].GetFloat();
        }
        if (materialData.HasMember("Diffuse"))
        {
            const auto& color = materialData["Diffuse"].GetArray();
            matData.material.diffuse.r = color[0].GetFloat();
            matData.material.diffuse.g = color[1].GetFloat();
            matData.material.diffuse.b = color[2].GetFloat();
            matData.material.diffuse.a = color[3].GetFloat();
        }
        if (materialData.HasMember("Specular"))
        {
            const auto& color = materialData["Specular"].GetArray();
            matData.material.specular.r = color[0].GetFloat();
            matData.material.specular.g = color[1].GetFloat();
            matData.material.specular.b = color[2].GetFloat();
            matData.material.specular.a = color[3].GetFloat();
        }
        if (materialData.HasMember("Emissive"))
        {
            const auto& color = materialData["Emissive"].GetArray();
            matData.material.emissive.r = color[0].GetFloat();
            matData.material.emissive.g = color[1].GetFloat();
            matData.material.emissive.b = color[2].GetFloat();
            matData.material.emissive.a = color[3].GetFloat();
        }
        if (materialData.HasMember("SpecularPower"))
        {
            matData.material.power = materialData["SpecularPower"].GetFloat();
        }
    }

    if (value.HasMember("Textures"))
    {
        const auto& textureData = value["Textures"].GetObj();
        if (textureData.HasMember("DiffuseMap"))
        {
            matData.diffuseMapName = textureData["DiffuseMap"].GetString();
        }
        if (textureData.HasMember("NormalMap"))
        {
            matData.normalMapName = textureData["NormalMap"].GetString();
        }
        if (textureData.HasMember("SpecMap"))
        {
            matData.specularMapName = textureData["SpecMap"].GetString();
        }
        if (textureData.HasMember("BumpMap"))
        {
            matData.bumpMapName = textureData["BumpMap"].GetString();
        }
    }
}

const Model& MeshComponent::GetModel() const
{
    return mModel;
}