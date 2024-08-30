#include "Precompiled.h"
#include "SoftBody.h"

#include "PhysicsWorld.h"

using namespace NotRed;
using namespace NotRed::Physics;
using namespace NotRed::Graphics;

SoftBody::~SoftBody()
{
	ASSERT(mSoftBody == nullptr, "SoftBody: terminate must be called");
}

void SoftBody::Initialize(Graphics::Mesh& mesh, float mass, const std::vector<uint32_t> fixedNodeIndices)
{
#ifdef USE_SOFT_BODY
	btSoftBodyWorldInfo& worldInfo = PhysicsWorld::Get()->GetSoftBodyWorldInfo();
	mSoftBody = new btSoftBody(&worldInfo, mesh.vertices.size(), nullptr, nullptr);
#else
	return;
#endif
	mMesh = &mesh;

	btSoftBody::Material* pm = mSoftBody->appendMaterial();
	pm->m_kAST = 1.0f;
	pm->m_kLST = 1.0f;
	pm->m_kVST = 1.0f;
	mSoftBody->m_cfg.kMT = 0.2f;
	mSoftBody->m_cfg.piterations = 10;
	mSoftBody->m_cfg.viterations = 10;
	mSoftBody->m_cfg.diterations = 10;
	mSoftBody->m_cfg.kCHR = 1.0f;
	mSoftBody->m_cfg.collisions |= btSoftBody::fCollision::SDF_RS;
	for (size_t i = 0; i < mMesh->vertices.size(); ++i)
	{
		mSoftBody->m_nodes[i].m_x = ConvertTobtVector3(mesh.vertices[i].position);
	}
	for (size_t i = 0; i < mesh.indices.size(); i += 6)
	{
		int index0 = mesh.indices[i];
		int index1 = mesh.indices[i + 4];
		int index2 = mesh.indices[i + 5];
		int index3 = mesh.indices[i + 2];

		mSoftBody->appendLink(index0, index1);
		mSoftBody->appendLink(index1, index2);
		mSoftBody->appendLink(index2, index3);
		mSoftBody->appendLink(index3, index0);
		mSoftBody->appendLink(index1, index3);
		mSoftBody->appendLink(index0, index2);
	}
	for (const int& fixedNode : fixedNodeIndices)
	{
		mSoftBody->m_nodes[fixedNode].m_im = 0.0f;
	}
	PhysicsWorld::Get()->Register(this);
}

void SoftBody::Terminate()
{
	PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mSoftBody);
}

void SoftBody::SyncGraphics()
{
	for (size_t i = 0; i < mMesh->vertices.size(); ++i)
	{
		mMesh->vertices[i].position = ConvertToVector3(mSoftBody->m_nodes[i].m_x);
	}
}