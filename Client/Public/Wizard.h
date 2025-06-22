#pragma once

#include "Monster.h"

class CWizard abstract : public CMonster
{
public:
	typedef struct tagWizardDesc : public CMonster::DESC
	{

	}DESC;

protected:
	CWizard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWizard(const CWizard& Prototype);
	virtual ~CWizard() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 상태패턴 넘겨줄 함수들 */
	_vector Get_State(STATE eState);

public:
	/* 이동 관련 */
	void Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed = 0.f, _float fMinDistance = 2.f);
	void Move(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Hit(_fvector vDir, _float fTimeDelta, _float fSpeed = 0.f);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed);

protected:
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration = 0.6f) override {};
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

