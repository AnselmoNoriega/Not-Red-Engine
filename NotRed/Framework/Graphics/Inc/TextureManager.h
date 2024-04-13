#pragma once

#include "Texture.h"

namespace NotRed::Graphics
{
    using TextureID = std::size_t;

    class TextureManager
    {
    public:
        static void Initialize(const std::filesystem::path& root);
        static void Terminate();
        static TextureManager* Get();

        TextureManager() = default;
        ~TextureManager();

        TextureManager(const TextureManager&) = delete;
        TextureManager(const TextureManager&&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&&) = delete;

        void SetRootDirectory(const std::filesystem::path& root);

        TextureID LoadTexture(const std::filesystem::path& fileName, bool useRootDir = true);
        const Texture* GetTexture(TextureID id);

        void BindVS(TextureID id, uint32_t slot) const;
        void BindPS(TextureID id, uint32_t slot) const;

    private:
        using Inventory = std::unordered_map<TextureID, std::unique_ptr<Texture>>;
        Inventory mInventory;

        std::filesystem::path mRootDirectory;
    };
}