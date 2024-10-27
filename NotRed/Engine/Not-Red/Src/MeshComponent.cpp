#include "Precompiled.h"
#include "MeshComponent.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
    RenderObjectComponent::Deserialize(value);
    Model::MeshData& meshData = mModel.meshData.emplace_back();
    Model::MeterialData& matData = mModel.meterialData.emplace_back();

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
    else
    {
        ASSERT(false, "MeshComponent: must have shape data");
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
            matData.diffuseMapName = textureData["NormalMap"].GetString();
        }
        if (textureData.HasMember("SpecMap"))
        {
            matData.diffuseMapName = textureData["SpecMap"].GetString();
        }
        if (textureData.HasMember("BumpMap"))
        {
            matData.diffuseMapName = textureData["BumpMap"].GetString();
        }
    }
}

const Model& MeshComponent::GetModel() const
{
    return mModel;
}