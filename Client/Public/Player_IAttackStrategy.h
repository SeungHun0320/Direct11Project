#pragma once

#include "Base.h"
#include "Client_Defines.h"

enum class ANIM_STATES;

BEGIN(Client)

class CPlayer_IAttackStrategy abstract : public CBase
{
public:
	CPlayer_IAttackStrategy() = default;
	virtual ~CPlayer_IAttackStrategy() = default;

public:
	virtual ANIM_STATES Get_AnimationState(int comboIndex) const = 0;
	virtual float Get_ComboDuration(int comboIndex) const = 0;
	virtual int Get_MaxComboCount() const = 0;

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

public:

public:
	virtual void Free();
};

END