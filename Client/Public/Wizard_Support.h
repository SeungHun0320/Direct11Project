#pragma once
#include "Monster.h"

BEGIN(Client)

class CWizard_Support final : public CMonster
{
public:
	typedef struct tagWizardSupportDesc : public CMonster::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_STAFF, PART_EFFECT, PART_END };

	enum ANIM_TYPE { IDLE, ATTACK, JUMP, ANIM_END };
	enum class STATES { IDLE, ATTACK, JUMP, HIT, STATES_END };

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

	/* 상태 관련 ?*/
	_float3 Get_Scaled();
	void Scaling(_float3 vScale);
	void Scaling(_float fX = 1.f, _float fY = 1.f, _float fZ = 1.f);

private: /* 상태 패턴 변수들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CWizard_SupportState* m_pCurState = { nullptr };
	class CWizard_SupportState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

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
