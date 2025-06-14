#pragma once
#include "Base.h"

BEGIN(Engine)

// 이벤트 신호를 주고받을 템플릿 클래스
template<typename... Args>
class CEvent_Signal final : public CBase
{
public:
    using DelType = Delegate<Args...>;
    vector<DelType> m_Listeners;
public:
    void Subscribe(const DelType& d) {
        m_Listeners.push_back(d);
    }
    void Publish(Args... args) {
        for (auto& Listener : m_Listeners)
            Listener.Execute(args...);
    }
};

END