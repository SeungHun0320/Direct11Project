#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPlayerState abstract : public CBase
{
public:
	CPlayerState(class CPlayer* pOwner) 
		: m_pOwner{ pOwner } { /* 약한 참조 ㅋㅋ */ }
	virtual ~CPlayerState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CPlayer* m_pOwner = { nullptr };

public:
	virtual void Free() override
	{
		__super::Free();
	}
};

class CPlayerState_Idle final : public CPlayerState
{
public:
	CPlayerState_Idle(class CPlayer* pOwner);
	virtual ~CPlayerState_Idle() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_Move final : public CPlayerState
{
public:
	CPlayerState_Move(class CPlayer* pOwner);
	virtual ~CPlayerState_Move() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CPlayerState_Dodge final : public CPlayerState
{
public:
	CPlayerState_Dodge(class CPlayer* pOwner);
	virtual ~CPlayerState_Dodge() = default;

public:	// CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float  m_fTimeAcc{};
	_bool   m_IsDodgeQueue{};
	_float3 m_vInputDir{};

public:
	virtual void Free()override;

};

class CPlayerState_Sprint final : public CPlayerState
{
public:
	CPlayerState_Sprint(class CPlayer* pOwner);
	virtual ~CPlayerState_Sprint() = default;

public:	// CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free()override;

};

class CPlayerState_Attack1 final : public CPlayerState
{
public:
	CPlayerState_Attack1(class CPlayer* pOwner);
	virtual ~CPlayerState_Attack1() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CPlayerState_Attack2 final : public CPlayerState
{
public:
	CPlayerState_Attack2(class CPlayer* pOwner);
	virtual ~CPlayerState_Attack2() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CPlayerState_Attack3 final : public CPlayerState
{
public:
	CPlayerState_Attack3(class CPlayer* pOwner);
	virtual ~CPlayerState_Attack3() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CPlayerState_Hit final : public CPlayerState
{
public:
	CPlayerState_Hit(class CPlayer* pOwner);
	virtual ~CPlayerState_Hit() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CPlayerState_Die final : public CPlayerState
{
public:
	CPlayerState_Die(class CPlayer* pOwner);
	virtual ~CPlayerState_Die() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

END