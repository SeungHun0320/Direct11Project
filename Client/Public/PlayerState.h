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

class CPlayerState_Move final : public CPlayerState
{
public:
	CPlayerState_Move(class CPlayer* pOwner);
	virtual ~CPlayerState_Move() = default;

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

class CPlayerState_Attack final : public CPlayerState
{
public:
	CPlayerState_Attack(class CPlayer* pOwner);
	virtual ~CPlayerState_Attack() = default;

public: // CPlayerState��(��) ���� ��ӵ�
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

public: // CPlayerState��(��) ���� ��ӵ�
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

public: // CPlayerState��(��) ���� ��ӵ�
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;


};

END