#pragma once

#include "Event.h"

namespace NotRed
{
	using ListenerId = uint32_t;
	using EventCallback = std::function<void(const Event*)>;

	class EventManager final
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static EventManager* Get();
		static void Broadcast(const Event* event);

		EventManager() = default;
		~EventManager();

		void Initialize();
		void Terminate();

		uint32_t AddListener(EventType eventType, const EventCallback& cb);
		void RemoveListener(EventType eventType, uint32_t listenerID);

	private:
		void BroadcastPrivate(const Event* event);

		using EventListeners = std::unordered_map<EventType, std::unordered_map<uint32_t, EventCallback>>;
		EventListeners mEventListeners;
		uint32_t mListenerId = 0;
	};
}