#pragma once
#include "BaseActor.h"

BEGIN(Client)

class CPlayer final : public CBaseActor
{
public:
	typedef struct tagPlayerDesc : public CBaseActor::DESC
	{
	}DESC;

public:
	enum PART { PART_BODY, PART_WEAPON,
		PART_HP, PART_STAMINA, PART_MP, PART_INVEN, PART_ITEMSLOTS, PART_POTION, PART_UIINVEN,
		PART_EFFECT, PART_END };

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
		ANIM_END
	};

	enum class STATES
	{
		IDLE, MOVE, DODGE, SPRINT, ATTACK1, ATTACK2, ATTACK3,
		HIT, GET_UP, DIE, PARRY, USE_POTION, DANCE, WAKE_UP,
		WIND_UP, TOSS, EAT, COIN_FLIP,
		OPEN_CHEST, ON_SWITCH, KNEEL,
		LADDER, ON_LADDER, OFF_LADDER, STATES_END
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public: /* 레벨 관련 */
	virtual void Set_Level(LEVEL eLevelID);
	void Set_SavePosition();
	void Clear_Target();
	void Change_Level();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CGameObject* Find_Target(_float fFindDistance);

public: /* 상태패턴 관련 함수들 */
	virtual void Change_States(STATES eStates);
	_vector Get_State(STATE eState);

public: /* 키 입력 관련 함수들*/
	_bool KeyDown(_ubyte eKeyID);
	_bool KeyPressing(_ubyte eKeyID);
	_bool KeyUp(_ubyte eKeyID);
	_bool IsAnyMoveKeyPressed() const;
	_bool IsMoveKeyPressed();
	_bool IsLockOn() const;
	
public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Set_TrackPosition(PART ePart, _float fTrackPosition);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_MeshVisible(PART ePart, _uint iIndex, _bool IsVisible);
	void  CheckChange_Anim(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);

	/* 충돌 관련 */
	void Set_Active(PART ePart, _bool isActive = true);
	void Set_Active(WEAPON_TYPE eType, _bool isActive = true);
	CCollider* Get_Collider(PART ePart, _uint iColliderIndex = 0);

	/* 이동 관련 */
	void  Dodge(_fvector vDir,_float fTimeDelta, _float fSpeed);
	void  Move(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void  Go_Dir(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void  Go_Up(_float fTimeDelta, _float fSpeed);
	void  Go_Down(_float fTimeDelta, _float fSpeed);
	void  LookTarget(_float fTimeDelta);

public: /* 상호작용 관련 */
	void  Change_States_ByInteract();
	void  Active_CheckPoint();
	void  Respawn();
	void  Use_Potion();
	void  Heal();

public: /* 공격 */
	void  Start_Attack();
	void  SetUp_AttackMeshVisible(WEAPON_TYPE eWeaponType);
	_bool DodgeComboAttack(ITEM_TYPE eItemType);
	_bool CheckDodgeComboWeapon();

public:/* 락온 관련 */
	void     LockOn();
	void     LockOff();
	void     LockOnMove(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	_vector  Get_TargetState(STATE eState);

public: /* 인벤토리 관련 */
	class CInventory* Get_Inventory() const;

	void Use_QuickSlot(_uint eSlot);
	void Equip_Weapon(class CPlayer_IAttackStrategy* pStrategy);
	void Use_Berrys(_byte byEatType);
	void Use_Coin_Question();
	void Use_FireCracker();

public: /* 키입력에 따른 방향을 결정해주는 함수 */
	_vector Get_InputDirection();
	_vector Get_InputDirectionEx();

public: /* 이벤트 매니저 관련 */
	void  Subscribe_Events();
	void  Equip_Shield();

public: /* 스테이트 갖고오기 */
	STATES Get_CurState() {	return m_eCurState; }
	STATES Get_PreState() { return m_ePreState; }

	_float Compute_StaggerValue() const;

	/* 스태미나 */
	_float Get_Stamina() const { return m_fStamina; }
	void Use_Stamina(_float fStamina);

	/* 마나 */
	void Use_Mana(_float fMana);

	/* 방패 관련 */
	_bool Has_Shield() const;

	/* 뭐 먹는중? */
	_byte Get_EatType() const { return m_byEatType; }

    /* 타깃 관련 */
	_bool Get_IsTarget() const { return m_isTarget; }
	_float Get_FindDistance() const { return m_fFindDistance; }

public: /* 전략패턴 트라이 */
	void Set_AttackStrategy(class CPlayer_IAttackStrategy* pStrategy);
	class CPlayer_IAttackStrategy* Get_AttackStrategy() const {
		return m_pAttackStrategy; }

/* 실제 플레이어 상태관련  */
private: /* 체력 */
	_float m_fHPRecorveryStat = {};
	_float m_fStaggerRecoveryPerSec = {};

private: /* 스태미나 */
	_float m_fStamina = {};
	_float m_fMaxStamina = {};
	_float m_fStaminaRecoveryPerSec = {};
	_float m_fStaminaTimeAcc = {};
	_bool  m_isUseStamina = { false };
	_float m_fStaminaDelayTimeAcc = {};
	_float m_fStaminaDelayTime = {};

private: /* 마나 */
	_float m_fMana = {};
	_float m_fMaxMana = {};
	_float m_fManaRecoveryStat = {};
	_float m_fManaTimeAcc = {};

private: /* 먹는 타입 */
	_byte m_byEatType = {};

private: /* 인벤 켰음? */
	_bool  m_isOnInven = { false };

private: /* 현재 충돌한 상호작용 오브젝트의 콜라이더 아이디 */
	COLLIDER_ID m_eCurInteractID = { COLLIDER_ID::CI_END };

private: /* 체크포인트 관련 변수 */
	_float3 m_vSavePosition = {};

private: /* 상태 패턴들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CPlayerState* m_pCurState = { nullptr };
	class CPlayerState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private: /* 전략 패턴 트라이 */
	class CPlayer_IAttackStrategy* m_pAttackStrategy = { nullptr };

private: /* 락온 상태관련 변수들 */
	class CMonster* m_pTarget = { nullptr };
	CTransform*		m_pTargetTransform = { nullptr };
	_bool			m_isTarget = { false };
	_float			m_fFindDistance = {};

private: /* 매번 캐스팅 해주기 싫어서 따로 변수로 받아왔음 */
	class CWeapon_Player*	  m_pWeaponPart = { nullptr };
	class CInventory*		  m_pInventory = { nullptr };

private: /* 실제 플레이어 상태 관련 */
	void Key_Input(_float fTimeDelta);
	void Stamina_Recovery(_float fTimeDelta);

private:
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual _float Compute_InvincibleTime(COLLIDER_ID eColliderID) override;

private: /* 애니 관련 */
	_bool Is_CurrentAnim(PART ePart, _uint iNextIndex);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	HRESULT Ready_States();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END