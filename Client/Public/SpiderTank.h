#pragma once

#include "Boss.h"

BEGIN(Client)

class CSpiderTank final : public CBoss
{
public:
	typedef struct tagSpiderTankDesc : public CBoss::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_HP, PART_LOCKON_START, PART_LOCKON_END, PART_STEAM, PART_EFFECT_BULLET, PART_END };

	enum ANIM_STATE {
		//NONE은 사용안함 ( 나중에 혹시 모름 )
		SLEEP, WAKE_UP,
		FORWARD, BACKWARD, RIGHT, LEFT, NONE1, NONE2,
		IDLE, KNOCKBACK, DEAD_START, DEAD, NONE3,
		NONE4, REVERSE, LAGER, SPAWNMOB,
		L_ATTACK, MID_ATTACK, R_ATTACK,
		NONE5, NONE6, R_SWING,
		READY_SHOT, SHOT, END_SHOT, NONE7, NONE8,
		FULLSWING, L_SWING,
		READY_BOMB, SHOT_BOMB, END_BOMB, PINCH
	};

	enum class STATES {
		SLEEP, WAKE_UP, IDLE,
		FORWARD, BACKWARD, RTURN, LTURN, REVERSE,
		LAGER, SPAWNMOB,
		FAST_ATTACK, SWING, FULLSWING,
		READY_SHOT, SHOT, END_SHOT,
		READY_BOMB, SHOT_BOMB, END_BOMB,
		PINCH, KNOCKBACK, DEAD, STATES_END
	};

	enum COL_TYPE { HEAD, WEAK, LEFT_ARM, RIGHT_ARM, COL_END };

private:
	CSpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderTank(const CSpiderTank& Prototype);
	virtual ~CSpiderTank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);
	_vector Get_State(STATE eState);

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Is_CurrentAnim(PART ePart, _uint iNextIndex);
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_TrackPosition(PART ePart, _float fTrackPosition);
	void  Set_TickPerSecond(PART ePart, _float fTickPerSecond);

	/* 충돌 관련 */
	void Set_Active(COL_TYPE eColTypeIndex, _bool isActive = true);

	/* 이동 관련 */
	_bool Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed = 0.f, _float fMinDistance = 2.f);
	void Go_Dir(_fvector vDir, _float fTimeDelta, _float fSpeed);
	void Move(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Hit(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed);
	void LookAtYaw(_fvector vDir, _float fLerpRatio);

	void Change_Camera(CAM_MODE eMode);

	_float Compute_AngleToPlayer();
	_float Compute_LookSppedByAngle(_float fAngle);
	_bool Is_TargetOnRight();

	/* 공격 관련 */
	HRESULT Shot_Bullet();
	const _bool Get_isShot() const { return m_isShot; }
	void    Set_isShot(_bool isShot) { m_isShot = isShot; }
	HRESULT Shot_Bomb();
	HRESULT Shot_Lager();

	void AttackCoolDownAcc(_float fTimeDelta) {	m_fAttackCoolDown += fTimeDelta; }
	_bool Is_AttackCoolDownReady(_float fAttackDelay) {	return m_fAttackCoolDown >= fAttackDelay; }
	void Reset_AttackCoolDown() { m_fAttackCoolDown = 0.f; }

	_uint Get_Sequence() { return m_iSequence; }
	void  Add_Sequence() { ++m_iSequence; }
	void  Reset_Sequence() { m_iSequence = 0; }

	void Set_isInBattle(_bool isInBattle) { m_isInBattle = isInBattle; }

private: /* 상태 패턴들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CSpiderTankState* m_pCurState = { nullptr };
	class CSpiderTankState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private:
	/* 쿨타임 */
	_float m_fAttackCoolDown = {};
	/* 서순 */
	_uint  m_iSequence = {};
private:
	_float m_fMuzzleAngle = {};

	_bool m_isInBattle = { false };
	_bool m_isShot = { false };
	

private: /* 필요해 */
	const _float4x4* m_pHeadBoneMatrix = { nullptr };
	_float4x4		 m_HeadBoneWorldMatrix = {};

	const _float4x4* m_pPowerCellBoneMatrix = { nullptr };
	_float4x4		 m_PowerCellBoneWorldMatrix = {};

private: /* 매번 캐스팅 해주기 싫어서 변수로 선언함 */
	class CBody_SpiderTank* m_pBodyPart = { nullptr };

private:
	void Update_BoneWorldMatrices(const _float4x4* pBoneMatrix, _float4x4* pBoneWorldMatrix);
	const _float4x4* Get_BoneMatrix(const _string& strBoneName);
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration = 0.6f) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	virtual HRESULT Ready_States() override;

public:
	static CSpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
