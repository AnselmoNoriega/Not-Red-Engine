#include "Precompiled.h"
#include "EventManager.h"

using namespace NotRed;

namespace
{
	std::unique_ptr<EventManager> sEventManager;
}

void EventManager::StaticInitialize()
{
	ASSERT(sEventManager == nullptr, "EventManager: is alreadt initialized");
	sEventManager = std::make_unique<EventManager>();
	sEventManager->Initialize();
}

void EventManager::StaticTerminate()
{
	if (sEventManager != nullptr)
	{
		sEventManager->Terminate();
		sEventManager.reset();
	}
}

EventManager* EventManager::Get()
{
	ASSERT(sEventManager != nullptr, "EventManager: is not initialized");
	return sEventManager.get();
}

void EventManager::Broadcast(const Event* event)
{
}

EventManager::~EventManager()
{
}

void EventManager::Initialize()
{
	mEventListeners.clear();
}

void EventManager::Terminate()
{
	mEventListeners.clear();
}

uint32_t EventManager::AddListener(EventType eventType, const EventCallback& cb)
{
	mEventListeners[eventType][++mListenerId] = cb;
	return mListenerId;
}

void EventManager::RemoveListener(EventType eventType, uint32_t listenerId)
{
	auto listenersIter = mEventListeners.find(eventType);
	if (listenersIter != mEventListeners.end())
	{
		auto iter = listenersIter->second.find(listenerId);
		if (iter != listenersIter->second.end())
		{
			listenersIter->second.erase(iter);
		}
	}
}

void EventManager::BroadcastPrivate(const Event* event)
{
	auto& listeners = mEventListeners[event->GetType()];
	for (auto& cb : listeners)
	{
		cb.second(event);
	}
}