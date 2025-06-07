#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CWizard_CandleabraState abstract : public CBase
{
public:
	CWizard_CandleabraState(class CWizard_Candleabra* pOwner)
		: m_pOwner{ pOwner } { }
	virtual ~CWizard_CandleabraState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CWizard_Candleabra* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

class CWizard_CandleabraState_Idle final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Idle(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Idle() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Detected final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Detected(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Detected() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fChaseDistance{};

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Guard final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Guard(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Guard() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fChaseDistance{};
	_float m_fAttackDistnace{};
	_float3 m_vTargetPos{};

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Attack final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Attack(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Attack() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fChaseDistance{};
	_float3 m_vTargetPos{};
	_float m_fAttackStartTime{};

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Move final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Move(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Move() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fMaxDistance{};
	_float3 m_vTargetPos{};

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Hit final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Hit(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Hit() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_CandleabraState_Dead final : public CWizard_CandleabraState
{
public:
	CWizard_CandleabraState_Dead(class CWizard_Candleabra* pOwner);
	virtual ~CWizard_CandleabraState_Dead() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

END