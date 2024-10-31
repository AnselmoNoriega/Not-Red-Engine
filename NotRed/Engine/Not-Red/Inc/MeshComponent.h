#pragma once

#include "RenderObjectComponent.h"

namespace NotRed
{
	class MeshComponent final : public RenderObjectComponent
	{
	public:
		SET_TYPE_ID(ComponentId::Mesh);
		void Deserialize(const rapidjson::Value& value) override;
		const Graphics::Model& GetModel() const override;

	private:
		Graphics::Model mModel;
	};
}