#pragma once

#include "Client_Defines.h"
#include "Base.h"

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
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
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

private:
	_float m_fHoldTime = {};

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

class CPlayerState_Attack abstract : public CPlayerState
{
public:
	CPlayerState_Attack(class CPlayer* pOwner)
		:CPlayerState{ pOwner } {};
	virtual ~CPlayerState_Attack() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override {};
	void Execute(_float fTimeDelta) override {};
	void Exit() override {};

protected:
	WEAPON_TYPE ToWeaponType(ITEM_TYPE eItemType)
	{
		switch (eItemType)
		{
		case ITEM_TYPE::STICK:
			return WEAPON_TYPE::STICK;
		case ITEM_TYPE::SWORD:
			return WEAPON_TYPE::SWORD;
		case ITEM_TYPE::DAGGER:
			return WEAPON_TYPE::DAGGER;
		default:
			return WEAPON_TYPE::WT_END;
		}
	}
	void Try_Combo(_uint iQuickSlotIdx);
	void Check_ComboKey();

protected:
	_uint  m_iMaxCombo = {};
	WEAPON_TYPE m_eWeaponType{};

	_bool  m_isAttackCombo = {};
	_float3 m_vInputDir{};

	_float m_fAttackStartTime = {};

public:
	virtual void Free() override {};
};

class CPlayerState_Attack1 final : public CPlayerState_Attack
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

class CPlayerState_Attack2 final : public CPlayerState_Attack
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

class CPlayerState_Attack3 final : public CPlayerState_Attack
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

private:
	_bool    m_isStagger = {};
	_float   m_fDownTime = {};
	_float3  m_vStaggerDir = {};
	
public:
	virtual void Free() override;
};

class CPlayerState_GetUp final : public CPlayerState
{
public:
	CPlayerState_GetUp(class CPlayer* pOwner);
	virtual ~CPlayerState_GetUp() = default;

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

class CPlayerState_Parry final : public CPlayerState
{
public:
	CPlayerState_Parry(class CPlayer* pOwner);
	virtual ~CPlayerState_Parry() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_WindUp final : public CPlayerState
{
public:
	CPlayerState_WindUp(class CPlayer* pOwner);
	virtual ~CPlayerState_WindUp() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_Toss final : public CPlayerState
{
public:
	CPlayerState_Toss(class CPlayer* pOwner);
	virtual ~CPlayerState_Toss() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_bool m_isToss{ false };

public:
	virtual void Free() override;
};

class CPlayerState_UsePotion final : public CPlayerState
{
public:
	CPlayerState_UsePotion(class CPlayer* pOwner);
	virtual ~CPlayerState_UsePotion() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_bool m_isUse = { false };

public:
	virtual void Free() override;
};

class CPlayerState_Eat final : public CPlayerState
{
public:
	CPlayerState_Eat(class CPlayer* pOwner);
	virtual ~CPlayerState_Eat() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_byte m_byEatType = {};

public:
	virtual void Free() override;
};

class CPlayerState_CoinFlip final : public CPlayerState
{
public:
	CPlayerState_CoinFlip(class CPlayer* pOwner);
	virtual ~CPlayerState_CoinFlip() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_OpenChest final : public CPlayerState
{
public:
	CPlayerState_OpenChest(class CPlayer* pOwner);
	virtual ~CPlayerState_OpenChest() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_OnSwitch final : public CPlayerState
{
public:
	CPlayerState_OnSwitch(class CPlayer* pOwner);
	virtual ~CPlayerState_OnSwitch() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_Kneel final : public CPlayerState
{
public:
	CPlayerState_Kneel(class CPlayer* pOwner);
	virtual ~CPlayerState_Kneel() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_WakeUp final : public CPlayerState
{
public:
	CPlayerState_WakeUp(class CPlayer* pOwner);
	virtual ~CPlayerState_WakeUp() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_Dance final : public CPlayerState
{
public:
	CPlayerState_Dance(class CPlayer* pOwner);
	virtual ~CPlayerState_Dance() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_Ladder final : public CPlayerState
{
public:
	CPlayerState_Ladder(class CPlayer* pOwner);
	virtual ~CPlayerState_Ladder() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_OnLadder final : public CPlayerState
{
public:
	CPlayerState_OnLadder(class CPlayer* pOwner);
	virtual ~CPlayerState_OnLadder() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CPlayerState_OffLadder final : public CPlayerState
{
public:
	CPlayerState_OffLadder(class CPlayer* pOwner);
	virtual ~CPlayerState_OffLadder() = default;

public: // CPlayerState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};





END