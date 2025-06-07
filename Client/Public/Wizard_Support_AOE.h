#pragma once

#include "Monster_Bullet_AOE.h"

class CWizard_Support_AOE final : public CMonster_Bullet_AOE
{
public:
	typedef struct tagWizardSupportAOEDesc : public CMonster_Bullet_AOE::DESC
	{

	}DESC;
private:
	CWizard_Support_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWizard_Support_AOE(const CWizard_Support_AOE& Prototype);
	virtual ~CWizard_Support_AOE() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CWizard_Support_AOE* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

