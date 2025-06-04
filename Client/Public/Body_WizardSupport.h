#pragma once

#include "Client_Defines.h"
#include "Body_Wizard.h"

#include "Wizard_Support.h"

BEGIN(Client)

class CBody_WizardSupport final : public CBody_Wizard
{
public:
	typedef struct tagBodySupportDesc : public CBody_Wizard::DESC
	{

	}DESC;
private:
	CBody_WizardSupport(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_WizardSupport(const CBody_WizardSupport& Prototype);
	virtual ~CBody_WizardSupport() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CWizard_Support::STATES* m_pParentState = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBody_WizardSupport* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END