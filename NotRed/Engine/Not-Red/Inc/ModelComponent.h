#pragma once

#include "RenderObjectComponent.h"

namespace NotRed
{
	class ModelComponent final : public RenderObjectComponent
	{
	public:
		SET_TYPE_ID(ComponentId::Model);

		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(const rapidjson::Value& value) override;

		Graphics::ModelID GetModelId() const override;
		const Graphics::Model& GetModel() const override;

	private:
		using Animations = std::vector<std::string>;

	private:
		std::string mFileName;
		Graphics::ModelID mModelId;

		Animations mAnimations;
	};
}