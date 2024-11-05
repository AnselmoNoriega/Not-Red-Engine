#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"

#include "SaveUtil.h"

#include "CameraService.h"
#include "RenderService.h"

using namespace NotRed;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld: is already initialized");
	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

	mInitialized = true;
}

void GameWorld::Terminate()
{
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
			slot.gameObject.reset();
		}
	}

	for (auto& service : mServices)
	{
		service->Terminate();
		service.reset();
	}
	mServices.clear();
}

void GameWorld::Update(float deltaTime)
{
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Update(deltaTime);
		}
	}

	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}

	ProcessDestroyList();
}
void GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}
void GameWorld::DebugUI()
{
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->DebugUI();
		}
	}

	for (auto& service : mServices)
	{
		service->DebugUI();
	}

	if (ImGui::Button("Save"))
	{
		SaveLevel();
	}
}
void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
	mLevelFileName = levelFile;
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.string().c_str(), "r");
	
	ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.string().c_str());
	
	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);
	
	rapidjson::Document doc;
	doc.ParseStream(readStream);
	
	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		std::string serviceName = service.name.GetString();
		Service* newService = nullptr;
		if (serviceName == "CameraService")
		{
			newService = AddService<CameraService>();
		}
		else if (serviceName == "RenderService")
		{
			newService = AddService<RenderService>();
		}
		else
		{
			ASSERT(false, "GameWorld: invalid service name %s", serviceName.c_str());
		}
		newService->Deserialize(service.value);
	}

	uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	auto gameObjects = doc["GameObjects"].GetObj();
	for (auto& gameObject : gameObjects)
	{
		std::string name = gameObject.name.GetString();
		std::string templateName = gameObject.value["Template"].GetString();
		GameObject* obj = CreateGameObject(name, templateName);
		GameObjectFactory::OverrideDeserialize(gameObject.value, *obj);
		obj->Initialize();
	}
}

void GameWorld::SaveLevel(std::filesystem::path saveFile)
{
	if (saveFile == "")
	{
		saveFile = mLevelFileName;
	}

	if (saveFile == "")
	{
		return;
	}

	rapidjson::Document doc;
	doc.SetObject();

	SaveUtil::SaveInt("Capacity", mGameObjectSlots.size(), doc, doc);
	rapidjson::Value services(rapidjson::kObjectType);

	for (auto& service : mServices)
	{
		service->Serialize(doc, services);
	}

	doc.AddMember("Services", services, doc.GetAllocator());
	rapidjson::Value gameObjects(rapidjson::kObjectType);

	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			rapidjson::Value gameObject(rapidjson::kObjectType);
			SaveUtil::SaveString("Template", slot.gameObject->mTemplateFilePath.c_str(), doc, gameObject);

			rapidjson::GenericStringRef<char> str(slot.gameObject->mName.c_str());
			gameObjects.AddMember(str, gameObject, doc.GetAllocator());
		}
	}
	doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

	FILE* file = nullptr;
	auto err = fopen_s(&file, saveFile.string().c_str(), "w");
	
	ASSERT(err == 0, "GameObject: failed to open template file %s", saveFile.u8string().c_str());
	char writeBuffer[65536];

	rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);

	writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
	doc.Accept(writer);

	fclose(file);
}

GameObject* GameWorld::CreateGameObject(std::string name, const std::filesystem::path& templatePath, bool initialize)
{
	ASSERT(mInitialized, "GameWorld: is not initialized");

	if (mFreeSlots.empty())
	{
		ASSERT(false, "GameWorld: no free slots available");
		return nullptr;
	}

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();
	Slot& slot = mGameObjectSlots[freeSlot];

	auto& newGameObject = slot.gameObject;
	newGameObject = std::make_unique<GameObject>();
	newGameObject->SetName(name);
	newGameObject->mWorld = this;
	newGameObject->mHandle.mIndex = freeSlot;
	newGameObject->mHandle.mGeneration = slot.generation;

	if (!templatePath.empty())
	{
		GameObjectFactory::Make(templatePath, *newGameObject);
		newGameObject->mTemplateFilePath = templatePath.string();
		if (initialize)
		{
			newGameObject->Initialize();
		}
	}

	return newGameObject.get();
}

void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return;
	}

	Slot& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
}

bool GameWorld::IsValid(const GameObjectHandle& handle)
{
	if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
	{
		return false;
	}
	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
	{
		return false;
	}

	return true;
}

void GameWorld::ProcessDestroyList()
{
	for (uint32_t index : mToBeDestroyed)
	{
		Slot& slot = mGameObjectSlots[index];
		GameObject* gameObject = slot.gameObject.get();

		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld: object is still alive");

		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mToBeDestroyed.clear();
}