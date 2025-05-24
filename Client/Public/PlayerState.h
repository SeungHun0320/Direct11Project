#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPlayerState abstract : public CBase
{
public:
	CPlayerState(class CPlayer* pOwner) 
		: m_pOwner{ pOwner }
	{ Safe_AddRef(m_pOwner); }
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
		Safe_Release(m_pOwner);
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

class CPlayerState_GoStraight final : public CPlayerState
{
public:
	CPlayerState_GoStraight(class CPlayer* pOwner);
	virtual ~CPlayerState_GoStraight() = default;

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

class CPlayerState_StickAttack final : public CPlayerState
{
public:
	CPlayerState_StickAttack(class CPlayer* pOwner);
	virtual ~CPlayerState_StickAttack() = default;



	// CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;

	void Execute(_float fTimeDelta) override;

	void Exit() override;

};

END