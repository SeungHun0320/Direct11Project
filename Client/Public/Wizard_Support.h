#pragma once
#include "Wizard.h"

BEGIN(Client)

class CWizard_Support final : public CWizard
{
public:
	typedef struct tagWizardSupportDesc : public CWizard::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_STAFF, PART_EFFECT, PART_END };

	enum ANIM_TYPE { IDLE, CASTING_START, PINCH, TELEPORT, FORWARD, CASTING, DEAD, ANIM_END };
	enum class STATES { IDLE, CASTING, HIT, TELEPORT, MOVE, DEAD, STATES_END };

private:
	CWizard_Support(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWizard_Support(const CWizard_Support& Prototype);
	virtual ~CWizard_Support() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);
	void    Set_State(STATE eState, _fvector vState);

	/* 공격 관련 */
	HRESULT Casting();

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);
	void  Set_TrackPosition(PART ePart, _float fTrackPosition);

	_float Get_CastingDistance() {
		return m_fCastingDistance;
	}

private: /* 상태 패턴 변수들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CWizard_SupportState* m_pCurState = { nullptr };
	class CWizard_SupportState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private: /* 이 놈 전용 변수들 */
	_float m_fCastingDistance = {};

protected: /* 충 돌 */
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration = 0.6f) override;
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	virtual HRESULT Ready_States() override;

public:
	static CWizard_Support* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
