#pragma once
#include "Base.h"
#include "Event_Signal.h"

BEGIN(Engine)

class CEvent_Manager : public CBase
{
private:
	CEvent_Manager() = default;
	virtual ~CEvent_Manager() = default;

public:
	template<typename... Args>
	HRESULT Subscribe(const _wstring& strEventTag, Delegate<Args...> dlg)
	{
		auto Iter = m_Events.find(strEventTag);

		// 처음 등록시 객체를 생성해서 맵에 추가
		if (Iter == m_Events.end())
		{
			CEvent_Signal<Args...>* pEvent = new CEvent_Signal<Args...>;
			pEvent->Subscribe(dlg);
			m_Events.emplace(strEventTag, move(pEvent));
		}
		// 이미 있으면 타입을 확인 후 구독자 추가
		else
		{
			CEvent_Signal<Args...>* pEvent = dynamic_cast<CEvent_Signal<Args...>*>(Iter->second);
			if (nullptr == pEvent)
			{
				MSG_BOX("이벤트 타입 불일치");
				return E_FAIL;
			}

			pEvent->Subscribe(dlg);
		}

		return S_OK;
	}

	template<typename... Args>
	void Publish(const _wstring& strEventTag, Args... args)
	{
		auto Iter = m_Events.find(strEventTag);
		if (Iter == m_Events.end())
			return;

		CEvent_Signal<Args...>* pEvent = dynamic_cast<CEvent_Signal<Args...>*>(Iter->second);
		if (nullptr == pEvent)
		{
			MSG_BOX("이벤트 타입 불일치");
			return;
		}

		pEvent->Publish(args...);
	}

	template<typename... Args>
	void Unsubscribe(void* pListener)
	{
		for (auto& [tag, pBaseEvent] : m_Events)
		{
			if (CEvent_Signal<Args...>* pStr = dynamic_cast<CEvent_Signal<Args...>*>(pBaseEvent))
				pStr->Unsubscribe(pListener);
		}
	}

private:
	// EventSignal 클래스는 템플릿 클래스이기 때문에 CBase로 관리
	unordered_map<_wstring, class CBase*> m_Events;

public:
	static CEvent_Manager* Create();
	virtual void Free();
};

END