#include "Precompiled.h"
#include "AudioSystem.h"

using namespace DirectX;
using namespace NotRed;
using namespace NotRed::Audio;

namespace
{
	std::unique_ptr<AudioSystem> sAudioSystem;
}

void Audio::AudioSystem::StaticInitialize()
{
	ASSERT(sAudioSystem == nullptr, "AudioSystem: is already initialized");
	sAudioSystem = std::make_unique<AudioSystem>();
	sAudioSystem->Initialize();
}

void Audio::AudioSystem::StaticTerminate()
{
	if (sAudioSystem != nullptr)
	{
		sAudioSystem->Terminate();
		sAudioSystem.reset();
	}
}

AudioSystem* Audio::AudioSystem::Get()
{
	ASSERT(sAudioSystem != nullptr, "AudioSystem is not initialized");
	return sAudioSystem.get();
}

Audio::AudioSystem::AudioSystem()
{
}

Audio::AudioSystem::~AudioSystem()
{
	ASSERT(mAudioEngine == nullptr, "AudioSystem: terminate must be called");
}

void Audio::AudioSystem::Initialize()
{
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;

#if defined(_DEBUG)
	flags |= AudioEngine_Debug;
#endif

	mAudioEngine = new AudioEngine(flags);
}

void Audio::AudioSystem::Terminate()
{
	if (mAudioEngine != nullptr)
	{
		delete mAudioEngine;
		mAudioEngine = nullptr;
	}
}

void Audio::AudioSystem::Update()
{
	if (mAudioEngine != nullptr && mAudioEngine->Update())
	{
		// if audio device not active
		if (mAudioEngine->IsCriticalError())
		{
			LOG("AudioSystem: critical error, shutting down");
			Terminate();
		}
	}
}

void Audio::AudioSystem::Suspend()
{
	ASSERT(mAudioEngine != nullptr, "AudioSystem: audio engine is not available");
	mAudioEngine->Suspend();
}