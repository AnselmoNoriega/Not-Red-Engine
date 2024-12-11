#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"

#include "Event.h"
#include "EventManager.h"

#include "SaveUtil.h"

// Game World
#include "GameWorld.h"

// Services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"
#include "PhysicsService.h"

// Game Object
#include "GameObject.h"
#include "GameObjectFactory.h"

// Components
#include "TypeIds.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "RenderObjectComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "VolumetricLightingComponent.h"

namespace NotRed
{
	App& MainApp();
}