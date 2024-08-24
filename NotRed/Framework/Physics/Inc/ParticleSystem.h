#pragma once

#include "Particle.h"

namespace NotRed::Physics
{
	struct ParticleSystemInfo
	{
		int maxParticle = 100;
		Graphics::TextureID particleTextureId = 0;
		Math::Vector3 spawnPosition = Math::Vector3::Zero;
		Math::Vector3 spawnDirection = Math::Vector3::YAxis;
		float spawnDelay = 0.0f;
		float spawnLifeTime = 0.0f;
		int minParticlesPerEmit = 0;
		int maxParticlesPerEmit = 0;
		float minTimeBetweenEmit = 0.0f;
		float maxTimeBetweenEmit = 0.0f;
		float minSpawnAngle = 0.0f;
		float maxSpawnAngle = 0.0f;
		float minSpeed = 0.0f;
		float maxSpeed = 0.0f;
		float minParticleLifeTime = 0.0f;
		float maxParticleLifeTime = 0.0f;
		Color minStartColor = Colors::White;
		Color maxStartColor = Colors::White;
		Color minEndColor = Colors::White;
		Color maxEndColor = Colors::White;
		Math::Vector3 minStartScale = Math::Vector3::One;
		Math::Vector3 maxStartScale = Math::Vector3::One;
		Math::Vector3 minEndScale = Math::Vector3::One;
		Math::Vector3 maxEndScale = Math::Vector3::One;
	};

	class ParticleSystem
	{
	public:
		void Initialize(const ParticleSystemInfo& info);
		void Terminate();
		void Update(float deltaTime);

		bool IsActive();
		void DebugUI();

		void SetPosition(const Math::Vector3& position);
		void SetCamera(const Graphics::Camera& camera);

		void SpawnParticles();

		template<class Effect>
		void Render(Effect& effect)
		{
			if (IsActive())
			{
				CurrentParticleInfo info;
				for (const int& index : mParticleIndices)
				{
					Particle* particle = mParticles[index].get();
					if (particle->IsActive())
					{
						particle->ObtainCurrentInfo(info);
						mParticleObject.transform = info.transform;
						effect.Render(mParticleObject, info.color);
					}
				}
			}
		}

	private:
		void InitializeParticles(uint32_t count);
		void SpawnSingleParticle();

		//particles
		using Particles = std::vector<std::unique_ptr<Particle>>;
		Particles mParticles;
		std::vector<int> mParticleIndices;

		//render info
		Graphics::RenderObject mParticleObject;
		const Graphics::Camera* mCamera = nullptr;

		//particle system info
		ParticleSystemInfo mInfo;
		int mNextAvailableParticleIndex;
		float mNextSpawn = 0.0f;
		float mLifeTime = 0.0f;
	};
}