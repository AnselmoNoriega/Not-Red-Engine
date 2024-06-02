#pragma once

#include "MeshTypes.h"

namespace NotRed::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(const std::filesystem::path& filename, float heightScale);

		float GetWidth() const;
		float GetLength() const;

		const Mesh& GetMesh() const;

		float GetHeightScale() const;
		float GetHeight(const Math::Vector3& position) const;

		void SetHeightScale(float heightScale);

	private:
		Mesh mMesh;
		uint32_t mRows = 0;
		uint32_t mColumns = 0;
		float mHeightScale = 1.0f;
	};
}