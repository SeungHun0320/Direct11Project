#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPlayer_IAttackStrategy abstract : public CBase
{
public:
	CPlayer_IAttackStrategy() = default;
	virtual ~CPlayer_IAttackStrategy() = default;

public:
	virtual _int Get_AnimationState(_int iComboIndex) const PURE;
	virtual _float Get_Duration(_int iComboIndex) const PURE;
	virtual _int Get_MaxComboCount() const PURE;
	virtual WEAPON_TYPE Get_WeaponType() const PURE;

public:
	virtual void Free() override
	{
		__super::Free();
	}

};

class CPlayer_SwordAttack final : public CPlayer_IAttackStrategy
{
public:
	CPlayer_SwordAttack() = default;
	virtual ~CPlayer_SwordAttack() = default;

public: // CPlayer_IAttackStrategy을(를) 통해 상속됨
	virtual _int Get_AnimationState(_int iComboIndex) const override;
	virtual _float Get_Duration(_int iComboIndex) const override;
	virtual _int Get_MaxComboCount() const override;
	WEAPON_TYPE Get_WeaponType() const override;

public:
	virtual void Free();
};

class CPlayer_StickAttack final : public CPlayer_IAttackStrategy
{
public:
	CPlayer_StickAttack() = default;
	virtual ~CPlayer_StickAttack() = default;

public: // CPlayer_IAttackStrategy을(를) 통해 상속됨
	virtual _int Get_AnimationState(_int iComboIndex) const override;
	virtual _float Get_Duration(_int iComboIndex) const override;
	virtual _int Get_MaxComboCount() const override;
	WEAPON_TYPE Get_WeaponType() const override;

public:
	virtual void Free();
};

END