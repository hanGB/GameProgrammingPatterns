#include "stdafx.h"
#include "event_dispatcher.h"

// static 변수 선언
std::vector<EventReciver*> EventDispatcher::m_recivers;
int EventDispatcher::m_maxPending = EventDispatcher::c_DEFAULT_MAX_PENDING;
EventData* EventDispatcher::m_pending = new EventData[EventDispatcher::m_maxPending];
int EventDispatcher::m_head = 0;
int EventDispatcher::m_tail = 0;

void EventDispatcher::AddReciver(EventReciver* reciver)
{
	m_recivers.push_back(reciver);
}

void EventDispatcher::RemoveAllRecivers()
{
	m_recivers.clear();
	m_recivers.resize(0);
}

void EventDispatcher::Update()
{
	if (m_head == m_tail) return;

	PEREvent event = m_pending[m_head].event;
	PERVec3 data = m_pending[m_head].data;
	for (auto& reciver : m_recivers) {
		reciver->Recive(event, data);
	}
	m_head = (m_head + 1) % m_maxPending;
}

void EventDispatcher::Send(PEREvent event, PERVec3 data)
{
	if ((m_tail + 1) % m_maxPending == m_head) ResizePending();

	m_pending[m_tail].event = event;
	m_pending[m_tail].data = data;
	m_tail = (m_tail + 1) % m_maxPending;
}

void EventDispatcher::ResizePending()
{
	// 배열 크기 2배로 변경
	m_maxPending *= 2;
	EventData* newPending = new EventData[m_maxPending];
	memmove(newPending, m_pending, sizeof(m_pending));

	delete[] m_pending;
	m_pending = newPending;

}
