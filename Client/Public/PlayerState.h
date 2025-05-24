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
	/* ���� ���� �� ȣ�� ( �̴ϼȶ����� ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* �ش� ������ �� �����Ӹ��� ȣ�� ( ������Ʈ ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* ���� ����ÿ� ( ����ÿ� �� �� ȣ�� ) */
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

public: // CPlayerState��(��) ���� ��ӵ�
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

public: // CPlayerState��(��) ���� ��ӵ�
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

public:	// CPlayerState��(��) ���� ��ӵ�
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

public:	// CPlayerState��(��) ���� ��ӵ�
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



	// CPlayerState��(��) ���� ��ӵ�
	void Enter(_float fTimeDelta) override;

	void Execute(_float fTimeDelta) override;

	void Exit() override;

};

END