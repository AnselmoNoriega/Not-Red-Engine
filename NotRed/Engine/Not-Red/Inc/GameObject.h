#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace NotRed
{
    class GameWorld;

    class GameObject final
    {
    private:
        using Components = std::vector<std::unique_ptr<Component>>;

    public:
        GameObject() = default;

        void Initialize();
        void Terminate();

        void Update(float deltaTime);
        void DebugUI();
        void SetName(std::string& name);

        const std::string GetName() const;
        uint32_t GetUniqueId() const;
        GameWorld& GetWorld() { return *mWorld; }
        const GameWorld& GetWorld() const { return *mWorld; }
        const GameObjectHandle& GetHandle() const { return mHandle; }

        template<class ComponentType>
        ComponentType* AddComponent()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component");

            ASSERT(!mInitialized, "GameObject: Cannot add components once initialized");
            ASSERT(ComponentType::StaticGetTypeId() != static_cast<uint32_t>(ComponentId::Invalid), "GameObject: invalid component type id");
            ASSERT(!HasA<ComponentType>(), "GameObject: already has a component type");

            auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
            newComponent->mOwner = this;

            return static_cast<ComponentType*>(newComponent.get());
        }

        template<class ComponentType>
        bool HasA()
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component");
            for (auto& component : mComponents)
            {
                if (component->GetTypeId() == ComponentType::StaticGetTypeId())
                {
                    return true;
                }
            }

            return false;
        }

        template<class ComponentType>
        const ComponentType* GetComponent() const
        {
            static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject: ComponentType must be of type Component");
            for (auto& component : mComponents)
            {
                if (component->GetTypeId() == ComponentType::StaticGetTypeId())
                {
                    return static_cast<ComponentType*>(component.get());
                }
            }

            return nullptr;
        }

        template<class ComponentType>
        ComponentType* GetComponent()
        {
            const GameObject* thisConst = static_cast<const GameObject*>(this);
            return const_cast<ComponentType*>(thisConst->GetComponent<ComponentType>());
        }

    private:
        void Save();

    private:
        std::string mName = "EMPTY";

        bool mInitialized = false;

        uint32_t mUniqueId = 0;
        Components mComponents;
        GameWorld* mWorld = nullptr;

        std::string mTemplateFilePath = "";
        GameObjectHandle mHandle;

    private:
        friend class GameWorld;
    };
}