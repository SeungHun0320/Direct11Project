#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CWizard_SupportState abstract : public CBase
{
public:
	CWizard_SupportState(class CWizard_Support* pOwner)
		: m_pOwner{ pOwner } { }
	virtual ~CWizard_SupportState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CWizard_Support* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

class CWizard_SupportState_Idle final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Idle(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Idle() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Detected final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Detected(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Detected() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Guard final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Guard(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Guard() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Attack final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Attack(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Attack() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Move final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Move(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Move() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Hit final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Hit(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Hit() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Dead final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Dead(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Dead() = default;

public: // CWizard_Candleabra을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

END