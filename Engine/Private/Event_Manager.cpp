#include "Event_Manager.h"
#include "Event_Signal.h"

CEvent_Manager* CEvent_Manager::Create()
{
	return new CEvent_Manager;
}

void CEvent_Manager::Free()
{
	__super::Free();

	for (auto& [key, value] : m_Events)
		Safe_Release(value);

	m_Events.clear();
}