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

public: // CWizard_SupportState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CWizard_SupportState_Casting final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Casting(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Casting() = default;

public: // CWizard_SupportState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_bool  m_isCasting = { false };
	_float m_fCastingTimeAcc = {};
	_float m_fCastingTime = {};
	_float3 m_vTargetDir{};

public:
	virtual void Free() override;
};

class CWizard_SupportState_Hit final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Hit(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Hit() = default;

public: // CWizard_SupportState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SupportState_Teleport final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Teleport(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Teleport() = default;

public: // CWizard_SupportState을(를) 통해 상속됨
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

public: // CWizard_SupportState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float3 m_vTargetDir{};

public:
	virtual void Free() override;
};

class CWizard_SupportState_Dead final : public CWizard_SupportState
{
public:
	CWizard_SupportState_Dead(class CWizard_Support* pOwner);
	virtual ~CWizard_SupportState_Dead() = default;

public: // CWizard_SupportState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};


END