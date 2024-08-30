#pragma once

#include "PhysicsObject.h"

namespace NotRed::Physics
{
	class SoftBody final : public PhysicsObject
	{
	public:
		SoftBody() = default;
		~SoftBody();
		void Initialize(Graphics::Mesh& mesh, float mass, const std::vector<uint32_t> fixedNodeIndices);
		void Terminate();

	protected:
		void SyncGraphics() override;
		btSoftBody* GetSoftBody() override { return mSoftBody; }

		btSoftBody* mSoftBody = nullptr;
		Graphics::Mesh* mMesh = nullptr;
	};
}