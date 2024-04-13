#include "Precompiled.h"
#include "TextureManager.h"

namespace NotRed::Graphics
{
    namespace
    {
        std::unique_ptr<TextureManager> sInstance;
    }

    void TextureManager::Initialize(const std::filesystem::path& root)
    {
        ASSERT(sInstance == nullptr, "Is already Initialized");
        sInstance = std::make_unique<TextureManager>();
        sInstance->SetRootDirectory(root);
    }

    void TextureManager::Terminate()
    {
        sInstance.reset();
    }

    TextureManager* TextureManager::Get()
    {
        ASSERT(sInstance != nullptr, "Is not Initialized");
        return sInstance.get();
    }

    TextureManager::~TextureManager()
    {
        for (auto&[id, texture] : mInventory)
        {
            texture->Terminate();
        }mInventory.clear();
    }

    void TextureManager::SetRootDirectory(const std::filesystem::path& root)
    {
        mRootDirectory = root;
    }

    TextureID TextureManager::LoadTexture(const std::filesystem::path& fileName, bool useRootDir)
    {
        const std::size_t textureID = std::filesystem::hash_value(fileName);
        auto [iter, success] = mInventory.insert({ textureID, nullptr });
        if (success)
        {
            auto& texturePtr = iter->second;
            texturePtr = std::make_unique<Texture>();
            texturePtr->Initialize((useRootDir) ? mRootDirectory / fileName : fileName);
        }

        return textureID;
    }

    const Texture* TextureManager::GetTexture(TextureID id)
    {
        auto iter = mInventory.find(id);
        if (iter != mInventory.end())
        {
            return iter->second.get();
        }

        return nullptr;
    }

    void TextureManager::BindVS(TextureID id, uint32_t slot) const
    {
        if (id == 0)
        {
            return;
        }

        auto iter = mInventory.find(id);
        if (iter != mInventory.end())
        {
            iter->second->BindVS(slot);
        }
    }

    void TextureManager::BindPS(TextureID id, uint32_t slot) const
    {
        if (id == 0)
        {
            return;
        }

        auto iter = mInventory.find(id);
        if (iter != mInventory.end())
        {
            iter->second->BindPS(slot);
        }
    }
}