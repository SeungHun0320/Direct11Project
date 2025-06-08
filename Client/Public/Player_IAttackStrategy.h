#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPlayer_IAttackStrategy abstract : public CBase
{
public:
	CPlayer_IAttackStrategy(_int iMaxCombo, WEAPON_TYPE eWeaponType)
		: m_iMaxCombo{ iMaxCombo }, m_eWeaponType{ eWeaponType } {};
	virtual ~CPlayer_IAttackStrategy() = default;

public:
	virtual _int Get_AnimationState(_int iComboIndex) const PURE;
	virtual _float Get_Duration(_int iComboIndex) const PURE;
	virtual _float Get_Attack() const PURE;
	virtual _float Get_StaggerValue() const PURE;
	virtual _int Get_MaxComboCount() const;
	virtual WEAPON_TYPE Get_WeaponType() const;

protected:
	WEAPON_TYPE m_eWeaponType{};
	_int m_iMaxCombo{};

public:
	virtual void Free() override;
};

class CPlayer_SwordAttack final : public CPlayer_IAttackStrategy
{
public:
	CPlayer_SwordAttack(_int iMaxCombo, WEAPON_TYPE eWeaponType)
		: CPlayer_IAttackStrategy{ iMaxCombo, eWeaponType } {};
	virtual ~CPlayer_SwordAttack() = default;

public: // CPlayer_IAttackStrategy을(를) 통해 상속됨
	virtual _int Get_AnimationState(_int iComboIndex) const override;
	virtual _float Get_Duration(_int iComboIndex) const override;
	virtual _float Get_Attack() const override;
	virtual _float Get_StaggerValue() const override;

public:
	virtual void Free();
};

class CPlayer_StickAttack final : public CPlayer_IAttackStrategy
{
public:
	CPlayer_StickAttack(_int iMaxCombo, WEAPON_TYPE eWeaponType)
		: CPlayer_IAttackStrategy{ iMaxCombo, eWeaponType } {
	};
	virtual ~CPlayer_StickAttack() = default;

public: // CPlayer_IAttackStrategy을(를) 통해 상속됨
	virtual _int Get_AnimationState(_int iComboIndex) const override;
	virtual _float Get_Duration(_int iComboIndex) const override;
	virtual _float Get_Attack() const override;
	virtual _float Get_StaggerValue() const override;

public:
	virtual void Free();
};

class CPlayer_DaggerAttack final : public CPlayer_IAttackStrategy
{
public:
	CPlayer_DaggerAttack(_int iMaxCombo, WEAPON_TYPE eWeaponType)
		: CPlayer_IAttackStrategy{ iMaxCombo, eWeaponType } {
	};
	virtual ~CPlayer_DaggerAttack() = default;

public: // CPlayer_IAttackStrategy을(를) 통해 상속됨
	virtual _int Get_AnimationState(_int iComboIndex) const override;
	virtual _float Get_Duration(_int iComboIndex) const override;
	virtual _float Get_Attack() const override;
	virtual _float Get_StaggerValue() const override;

public:
	virtual void Free();
};

END