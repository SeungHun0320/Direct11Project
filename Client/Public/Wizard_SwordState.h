#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CWizard_SwordState abstract : public CBase
{
public:
	CWizard_SwordState(class CWizard_Sword* pOwner)
		: m_pOwner{ pOwner } { }
	virtual ~CWizard_SwordState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CWizard_Sword* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

class CWizard_SwordState_Idle final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Idle(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Idle() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CWizard_SwordState_Pray final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Pray(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Pray() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CWizard_SwordState_Detected final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Detected(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Detected() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SwordState_Attack final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Attack(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Attack() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_byte m_byRandom{};
	_float3 m_vTargetPos{};
	_float m_fAttackStartTime{};

public:
	virtual void Free() override;
};


class CWizard_SwordState_Move final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Move(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Move() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float3 m_vTargetPos{};

public:
	virtual void Free() override;
};

class CWizard_SwordState_Hit final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Hit(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Hit() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CWizard_SwordState_Dead final : public CWizard_SwordState
{
public:
	CWizard_SwordState_Dead(class CWizard_Sword* pOwner);
	virtual ~CWizard_SwordState_Dead() = default;

public: // CWizard_SwordState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

END