#pragma once
#include "per_event.h"
#include "event_reciver.h"
#include "cs_provider.h"

struct EventData {
	PEREvent event;
	PERVec3 data;
};

class EventDispatcher {
public:
	static void SetGame(EventReciver* reciver);

	static void AddReciver(EventReciver* reciver);
	static void RemoveReciver(EventReciver* reciver);
	static void RemoveAllRecivers();

	static void Update();
	static void Send(PEREvent pevent, PERVec3 data);

	static const int c_DEFAULT_MAX_PENDING = 516;

private:
	static void ResizePending();

	static EventReciver* m_game;
	static std::vector<EventReciver*> m_recivers;

	static CSProvider m_csProvider;

	static int m_maxPending;
	static EventData* m_pending;
	static int m_head;
	static int m_tail;
};