#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayer final : public CPawn
{
public:
	typedef struct tagPlayerDesc : public CPawn::DESC {


	}DESC;

public:
	enum ANIM_STATES
	{
		IDLE,
		GO_STRAIGHT, GO_LEFT, GO_RIGHT, GO_BACK,
		OPEN_CHEST,
		LADDER, ON_LADDER, OFF_LADDER,
		PARRY,
		KNEEL,
		EAT,USE_POTION,
		ON_SWITCH,
		COIN_FLIP,
		WINDUP,	TOSS,
		WAKE_UP, DANCE,
		DODGE, FAIL_DODGE, HYPERDASH,
		HIT, STAGGER, GET_UP,
		DIE,
		DAGGER,
		STICK_ATTACK1, STICK_ATTACK2,
		SWORD_ATTACK1, SWORD_ATTACK2, SWORD_ATTACK3,
		SPRINT,
		PS_END
	};

	enum class STATES
	{
		IDLE, MOVE, DODGE, ATTACK, HIT, DIE, ST_END
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual void Set_Level(LEVEL eLevelID) {
		__super::Set_Level(eLevelID);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:


private:
	STATES m_eCurState{ STATES::ST_END };
	STATES m_ePreState{ STATES::ST_END };
	class CPlayerState* m_pCurState = { nullptr };
	class CPlayerState* m_pStates[ENUM_CLASS(STATES::ST_END)];

private:
	void Key_Input(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT Ready_States();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END