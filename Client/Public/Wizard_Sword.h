#pragma once
#include "Monster.h"

BEGIN(Client)

class CWizard_Sword final : public CMonster
{
public:
	typedef struct tagWizardSwordDesc : public CMonster::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_SWORD, PART_EFFECT, PART_END };

	enum ANIM_TYPE { IDLE, ATTACK1, ATTACK2, PRAY, PINCH, DETECTED, FORWARD, BACKWARD, RIGHT, LEFT, NONE, DEAD, ANIM_END };
	enum class STATES { IDLE, PRAY, DETECTED, ATTACK, MOVE, HIT, DEAD, STATES_END };

private:
	CWizard_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWizard_Sword(const CWizard_Sword& Prototype);
	virtual ~CWizard_Sword() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 충 돌 */
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID);

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);
	_vector Get_State(STATE eState);

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_TrackPosition(PART ePart, _float fTrackPosition);

	/* 이동 관련 */
	void Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed = 0.f, _float fMinDistance = 2.f);
	void Move(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Hit(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed);

private: /* 상태 패턴 변수들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CWizard_SwordState* m_pCurState = { nullptr };
	class CWizard_SwordState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	virtual HRESULT Ready_States() override;

public:
	static CWizard_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
