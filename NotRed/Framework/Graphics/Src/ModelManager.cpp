#include "Precompiled.h"
#include "ModelManager.h"
#include "ModeliO.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
    std::unique_ptr<ModelManager> sModelManager;
}

void ModelManager::StaticInitialize()
{
    ASSERT(sModelManager == nullptr, "ModelManager is already initialized");
    sModelManager = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
    sModelManager.reset();
}

ModelManager* ModelManager::Get()
{
    ASSERT(sModelManager != nullptr, "Modelmanager: was not initialized");

    return sModelManager.get();
}

ModelID ModelManager::GetModelID(const std::filesystem::path& filepath)
{
    return std::filesystem::hash_value(filepath);
}

ModelID ModelManager::LoadModel(const std::filesystem::path& filepath)
{
    const ModelID modelID = GetModelID(filepath);
    auto [iter, success] = mInventory.insert({ modelID, nullptr });
    if (success)
    {
        auto& modelPtr = iter->second;
        modelPtr = std::make_unique<Model>();
        ModelIO::LoadModel(filepath, *modelPtr);
        ModelIO::LoadMaterial(filepath, *modelPtr);
        ModelIO::LoadSkeleton(filepath, *modelPtr);
    }
    return modelID;
}

void ModelManager::AddAnimation(ModelID id, const std::filesystem::path& filepath)
{
    auto model = mInventory.find(id);
    ASSERT(model != mInventory.end(), "ModelManager: model was not loaded for animation");
    ModelIO::LoadAnimations(filepath, *model->second);
}

const Model* ModelManager::GetModel(ModelID id)
{
    auto model = mInventory.find(id);
    if (model != mInventory.end())
    {
        return model->second.get();
    }
    return nullptr;
}