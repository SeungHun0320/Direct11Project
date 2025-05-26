#pragma once
#include "Pawn.h"

BEGIN(Client)

class CPlayer final : public CPawn
{
public:
	typedef struct tagPlayerDesc : public CPawn::DESC {

		_int iStamina;

	}DESC;

public:
	enum MESHES	{
		MESH_SHILED, MESH_SWORD, MESH_POTION, MESH_POTION2, MESH_STICK, MESH_DAGGER, MESHES_END
	};
	enum ANIM_STATES
	{
		IDLE, GO_STRAIGHT, GO_LEFT, GO_RIGHT, GO_BACK,
		OPEN_CHEST,
		LADDER, ON_LADDER, OFF_LADDER,
		PARRY,
		KNEEL,
		EAT, USE_POTION,
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
		IDLE, MOVE, DODGE, SPRINT, ATTACK1, ATTACK2, ATTACK3,
		HIT, GET_UP, DIE, PARRY, USE_POTION, DANCE, WAKE_UP,
		WIND_UP, TOSS, ST_END
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

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);
	_vector Get_State(STATE eState);

public: /* 키 입력 관련 함수들*/
	_bool KeyDown(_ubyte eKeyID);
	_bool KeyPressing(_ubyte eKeyID);
	_bool KeyUp(_ubyte eKeyID);
	_bool IsAnyMoveKeyPressed() const;
	
public: /* 상태로 넘겨줄 함수들 */
	_bool Play_Animation(_float fTimeDelta);
	void  Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_MeshVisible(_uint iIndex, _bool IsVisible);
	void  Dodge(_fvector vDir,_float fTimeDelta, _float fSpeed);
	void  Move(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void  Stagger(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);

public: /* 키입력에 따른 룩 갖고오는 함수 */
	_vector Get_InputDirection();

public: /* 스테이트 갖고오기 */
	STATES Get_CurState() {
		return m_eCurState;
	}
	STATES Get_PreState() {
		return m_ePreState;
	}

public: /* 스태미나 */
	_float Get_Stamina() const {
		return m_fStamina;
	}
	void Use_Stamina(_float fStamina) {
		m_fStamina -= fStamina;
	}

public: /* 피격 관련 */
	HIT_TYPE Get_HitType() const {
		return m_eHitType;
	}
	void Set_HitType(HIT_TYPE eHitType) {
		m_eHitType = eHitType;
	}
	_bool Get_IsHit() const {
		return m_IsHit;
	}
	void Set_Hit(_bool IsHit) {
		m_IsHit = IsHit;
	}
public: /* 방패 관련 */
	_bool Get_IsShield() {
		return m_IsShield;
	}
	void Set_IsShield(_bool IsShield) {
		m_IsShield = IsShield;
	}

public: /* 전략패턴 트라이 */
	void Set_AttackStrategy(class CPlayer_IAttackStrategy* pStrategy);
	class CPlayer_IAttackStrategy* Get_AttackStrategy() const {
		return m_pAttackStrategy;
	};

private:
	void Key_Input(_float fTimeDelta);

private: /* 스태미나 */
	_float m_fStamina = {};
	_float m_fMaxStamina = {};
	_float m_fStaminaRecoveryPerSec = {};
	_float m_fStaminaTimeAcc = {};

private: /* 방패를 소유하고 있는지 아닌지 */
	_bool m_IsShield = { false };

private: /* 상태 패턴들 */
	STATES m_eCurState{ STATES::ST_END };
	STATES m_ePreState{ STATES::ST_END };
	class CPlayerState* m_pCurState = { nullptr };
	class CPlayerState* m_pStates[ENUM_CLASS(STATES::ST_END)] = { nullptr };

private: /* 전략 패턴 트라이*/
	class CPlayer_IAttackStrategy* m_pAttackStrategy = { nullptr };
	HIT_TYPE    m_eHitType = {};
	_bool       m_IsHit = {};

private:
	void Stamina_Recovery(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT Ready_States();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END