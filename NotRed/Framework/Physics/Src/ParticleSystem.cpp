#include "Precompiled.h"
#include "ParticleSystem.h"

using namespace NotRed;
using namespace NotRed::Physics;
using namespace NotRed::Math;
using namespace NotRed::Graphics;

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailableParticleIndex = 0;
	mNextSpawn = info.spawnDelay;
	mLifeTime = info.spawnLifeTime;

	Mesh particleMesh = MeshBuilder::CreateSpriteQuad(0.5f, 0.5f);
	mParticleObject.meshBuffer.Initialize(particleMesh);
	mParticleObject.diffuseMapID = info.particleTextureId;

	InitializeParticles(info.maxParticle);
}

void ParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndices.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndices[i] = i;
		mParticles[i] = std::make_unique<Particle>();
		mParticles[i]->Initialize();
	}
}

void ParticleSystem::Terminate()
{
	mParticleObject.Terminate();
	for (auto& p : mParticles)
	{
		p->Terminate();
		p.reset();
	}
}

void ParticleSystem::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		mNextSpawn -= deltaTime;
		if (mNextSpawn <= 0.0f && mLifeTime > 0.0f)
		{
			SpawnParticles();
		}
		for (auto& p : mParticles)
		{
			p->Update(deltaTime);
		}

		/*std::sort(mParticleIndices.begin(), mParticleIndices.end(), [&](const int& a, const int& b)
			{
				float distSqrA = MagnitudeSqr(mParticles[a]->GetPosition() - mCamera->GetPosition());
				float distSqrB = MagnitudeSqr(mParticles[b]->GetPosition() - mCamera->GetPosition());
				return distSqrA < distSqrB;
			});*/
	}
}

bool ParticleSystem::IsActive()
{
	if (mLifeTime > 0.0f)
	{
		return true;
	}

	for (auto& p : mParticles)
	{
		if (p->IsActive())
		{
			return true;
		}
	}

	return false;
}

void ParticleSystem::DebugUI()
{
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		ImGui::DragFloat3("Position", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("Direction##Particle", &mInfo.spawnDirection.x, 0.05))
		{
			mInfo.spawnDirection = Normalize(mInfo.spawnDirection);
		}
		ImGui::DragInt("MinPerEmit", &mInfo.minParticlesPerEmit);
		mInfo.maxParticlesPerEmit = Max(mInfo.minParticlesPerEmit, mInfo.maxParticlesPerEmit);
		ImGui::DragInt("MaxPerEmit", &mInfo.maxParticlesPerEmit, 1, mInfo.minParticlesPerEmit);
		ImGui::DragFloat("MinTimeToEmit", &mInfo.minTimeBetweenEmit, 0.01f);
		ImGui::DragFloat("MaxTimeToEmit", &mInfo.maxTimeBetweenEmit, 0.01f, mInfo.minTimeBetweenEmit);
		ImGui::DragFloat("MinAngle", &mInfo.minSpawnAngle, 0.001f, -Math::Constants::Pi);
		ImGui::DragFloat("MaxAngle", &mInfo.maxSpawnAngle, 0.001f, mInfo.minSpawnAngle);
		ImGui::DragFloat("MinSpeed", &mInfo.minSpeed);
		ImGui::DragFloat("MaxSpeed", &mInfo.maxSpeed, 1.0f, mInfo.minSpeed);
		ImGui::DragFloat("MinLife", &mInfo.minParticleLifeTime, 0.25f);
		ImGui::DragFloat("MaxLife", &mInfo.maxParticleLifeTime, 0.25f, mInfo.minParticleLifeTime);
		ImGui::ColorEdit4("MinStartColor", &mInfo.minStartColor.r);
		ImGui::ColorEdit4("MaxStartColor", &mInfo.maxStartColor.r);
		ImGui::ColorEdit4("MinEndColor", &mInfo.minEndColor.r);
		ImGui::ColorEdit4("MaxEndColor", &mInfo.maxEndColor.r);
		ImGui::DragFloat3("MinStartScale", &mInfo.minStartScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MaxStartScale", &mInfo.maxStartScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MinEndScale", &mInfo.minEndScale.x, 0.1f, 0.001f);
		ImGui::DragFloat3("MaxEndScale", &mInfo.maxEndScale.x, 0.1f, 0.001f);
	}
}

void ParticleSystem::SetPosition(const Math::Vector3& position)
{
	mInfo.spawnPosition = position;
}

void ParticleSystem::SetCamera(const Graphics::Camera& camera)
{
	mCamera = &camera;
}

void ParticleSystem::SpawnParticles()
{
	int numParticles = mInfo.minParticlesPerEmit + (rand() % (mInfo.maxParticlesPerEmit - mInfo.minParticlesPerEmit + 1));
	for (int i = 0; i < numParticles; ++i)
	{
		SpawnSingleParticle();
	}
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	mNextSpawn = mInfo.minTimeBetweenEmit + ((mInfo.maxTimeBetweenEmit - mInfo.minTimeBetweenEmit) * randFloat);
}

void ParticleSystem::SpawnSingleParticle()
{
	Particle* particle = mParticles[mNextAvailableParticleIndex].get();

	mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();

	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randAngle = mInfo.minSpawnAngle + ((mInfo.maxSpawnAngle - mInfo.minSpawnAngle) * randFloat);

	// rotAngle0 = cross product with up and desired angle (may have case where up and angle are same, resolve)
	// rotAngle1 = cross produce rotAngle0 and up
	// mult angle matrices together
	Vector3 rotAxis = (Abs(Dot(mInfo.spawnDirection, Vector3::YAxis)) >= 0.9f) ? Vector3::XAxis : Vector3::YAxis;
	Matrix4 matRotA = Matrix4::RotationAxis(rotAxis, randAngle);

	rotAxis = Vector3::ZAxis;// Normalize(Cross(rotAxis, mInfo.spawnDirection));
	Matrix4 matRot = Matrix4::RotationAxis(rotAxis, randAngle) * matRotA;
	Vector3 spawnDirection = TransformNormal(mInfo.spawnDirection, matRot);

	randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);

	ParticleActivationData activateData;

	float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.startColor = Lerp(mInfo.minStartColor, mInfo.maxStartColor, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.endColor = Lerp(mInfo.minEndColor, mInfo.maxEndColor, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.endScale = Lerp(mInfo.minEndScale, mInfo.maxEndScale, t);
	t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	activateData.lifeTime = Lerp(mInfo.minParticleLifeTime, mInfo.maxParticleLifeTime, t);
	activateData.position = mInfo.spawnPosition;
	activateData.velocity = spawnDirection * speed;
	particle->Activate(activateData);
}