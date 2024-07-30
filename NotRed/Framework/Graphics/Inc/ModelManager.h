#pragma once

#include "Model.h"

namespace NotRed::Graphics
{
	using ModelID = std::size_t;
	class ModelManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

		ModelManager() = default;
		~ModelManager() = default;

		ModelManager(const ModelManager&) = delete;
		ModelManager(const ModelManager&&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&&) = delete;

		ModelID GetModelID(const std::filesystem::path& filepath);
		ModelID LoadModel(const std::filesystem::path& filepath);
		const Model* GetModel(ModelID id);

	private:
		using ModelCache = std::map<ModelID, std::unique_ptr<Model>>;
		ModelCache mInventory;
	};
}