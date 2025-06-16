#pragma once
#include "Wizard.h"

BEGIN(Client)

class CWizard_Sword final : public CWizard
{
public:
	typedef struct tagWizardSwordDesc : public CWizard::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_SWORD, PART_HP, PART_EFFECT, PART_END };

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

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_TrackPosition(PART ePart, _float fTrackPosition);

	/* 충돌 관련 */
	void Set_Active(_bool isActive = true);

private: /* 상태 패턴 변수들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CWizard_SwordState* m_pCurState = { nullptr };
	class CWizard_SwordState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

protected: /* 충 돌 */
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration = 0.6f) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

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
