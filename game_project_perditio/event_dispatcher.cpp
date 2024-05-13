#include "stdafx.h"
#include "event_dispatcher.h"

// static 변수 선언
EventReciver* EventDispatcher::m_game;
std::vector<EventReciver*> EventDispatcher::m_recivers;

CSProvider EventDispatcher::m_csProvider = CSProvider();

int EventDispatcher::m_maxPending = EventDispatcher::c_DEFAULT_MAX_PENDING;
EventData* EventDispatcher::m_pending = new EventData[EventDispatcher::m_maxPending];
int EventDispatcher::m_head = 0;
int EventDispatcher::m_tail = 0;

void EventDispatcher::SetGame(EventReciver* reciver)
{
	m_csProvider.Lock();

	m_game = reciver;
	AddReciver(m_game);

	m_csProvider.Unlock();
}

void EventDispatcher::AddReciver(EventReciver* reciver)
{
	m_csProvider.Lock();

	m_recivers.push_back(reciver);

	m_csProvider.Unlock();
}

void EventDispatcher::RemoveReciver(EventReciver* reciver)
{
	m_csProvider.Lock();

	auto it = m_recivers.begin();
	while (true) {
		if ((*it) == reciver) {
			m_recivers.erase(it);
			break;
		}
		it++;
	}

	m_csProvider.Unlock();
}

void EventDispatcher::RemoveAllRecivers()
{
	m_csProvider.Lock();

	m_recivers.clear();
	m_recivers.resize(0);

	// 무조건 게임 클래스는 이벤트를 받아야 함
	AddReciver(m_game);

	m_csProvider.Unlock();
}

void EventDispatcher::Update()
{
	m_csProvider.Lock();

	if (m_head == m_tail) {
		m_csProvider.Unlock();
		return;
	}

	PEREvent event = m_pending[m_head].event;
	PERVec3 data = m_pending[m_head].data;
	for (auto& reciver : m_recivers) {
		reciver->Recive(event, data);
	}
	m_head = (m_head + 1) % m_maxPending;

	m_csProvider.Unlock();
}

void EventDispatcher::Send(PEREvent event, PERVec3 data)
{
	m_csProvider.Lock();

	if ((m_tail + 1) % m_maxPending == m_head) ResizePending();

	m_pending[m_tail].event = event;
	m_pending[m_tail].data = data;
	m_tail = (m_tail + 1) % m_maxPending;

	m_csProvider.Unlock();
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
