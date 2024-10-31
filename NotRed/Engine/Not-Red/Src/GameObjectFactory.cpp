#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

namespace
{
	Component* AddComponent(const std::string& componentName, GameObject& gameObject)
	{
		Component* newComponent = nullptr;
		if (componentName == "TransformComponent")
		{
			newComponent = gameObject.AddComponent<TransformComponent>();
		}
		else if (componentName == "CameraComponent")
		{
			newComponent = gameObject.AddComponent<CameraComponent>();
		}
		else if (componentName == "FPSCameraComponent")
		{
			newComponent = gameObject.AddComponent<FPSCameraComponent>();
		}
		else if (componentName == "MeshComponent")
		{
			newComponent = gameObject.AddComponent<MeshComponent>();
		}
		else
		{
			ASSERT(false, "Unknown COmponent Type");
		}

		return newComponent;
	}
}

namespace NotRed::GameObjectFactory
{
	void Make(const std::filesystem::path& templatePath, GameObject& gameObject)
	{
		FILE* file = nullptr;
		auto err = fopen_s(&file, templatePath.string().c_str(), "r");
		ASSERT(err == 0, "GameObjectFactory: Faild to open file");

		char readBuffer[65536];
		rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
		fclose(file);

		rapidjson::Document doc;
		doc.ParseStream(readStream);
		auto components = doc["Components"].GetObj();
		for (auto& component : components)
		{
			Component* newComponent = AddComponent(component.name.GetString(), gameObject);
			if (newComponent != nullptr)
			{
				newComponent->Deserialize(component.value);
			}
		}
	}
}