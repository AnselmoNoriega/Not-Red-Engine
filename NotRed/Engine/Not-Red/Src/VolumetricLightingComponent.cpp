#include "Precompiled.h"
#include "VolumetricLightingComponent.h"

#include "RenderService.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void VolumetricLightComponent::Initialize()
{
    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->Register(this);
    
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void VolumetricLightComponent::Terminate()
{
    mLightObj.Terminate();
    mInLightObj.Terminate();
}

void VolumetricLightComponent::Update(float deltaTime)
{
    mLight.TimeRunning += deltaTime * mTimeSpeed;

    if (mTransformComponent)
    {
        mLight.CameraObj.SetPosition(mTransformComponent->position + mLight.LightPosition);
        mLightObj.transform.position = mTransformComponent->position;
        mInLightObj.transform.position = mTransformComponent->position;
    }
}

void VolumetricLightComponent::Deserialize(const rapidjson::Value& value)
{
    const auto& shapeData = value["Shape"].GetObj();
    uint32_t shapeSlices = shapeData["SlicesCount"].GetUint();
    float shapeHeigth = shapeData["Heigth"].GetFloat();
    float shapeRadius = shapeData["Radius"].GetFloat();

    {
        const MeshPC& m = NotRed::Graphics::MeshBuilder::CreateCone(shapeSlices, shapeHeigth, shapeRadius);   // Max offset);
        mLightObj.meshBuffer.Initialize(
            m.vertices.data(),
            static_cast<uint32_t>(sizeof(VertexPC)),
            static_cast<uint32_t>(m.vertices.size()),
            m.indices.data(),
            static_cast<uint32_t>(m.indices.size())
        );
    }
    {
        const MeshPC& m = NotRed::Graphics::MeshBuilder::CreateInCone(shapeSlices, shapeHeigth, shapeRadius);
        mInLightObj.meshBuffer.Initialize(
            m.vertices.data(),
            static_cast<uint32_t>(sizeof(VertexPC)),
            static_cast<uint32_t>(m.vertices.size()),
            m.indices.data(),
            static_cast<uint32_t>(m.indices.size())
        );
    }

    const auto& lightData = value["Light"].GetObj(); 
    const auto lightColor = lightData["Color"].GetArray();
    mLight.LightColor = Math::Vector3(lightColor[0].GetFloat(), lightColor[1].GetFloat(), lightColor[2].GetFloat());
    mLight.LightPosition = Math::Vector3(0.0f, shapeHeigth, 0.0f);
}

void NotRed::VolumetricLightComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("Ligght Data:"))
    {
        ImGui::DragFloat("Time Speed", &mTimeSpeed, 0.02f);
        ImGui::ColorEdit3("Light Color", &mLight.LightColor.x, 0.05f);
    }
}
