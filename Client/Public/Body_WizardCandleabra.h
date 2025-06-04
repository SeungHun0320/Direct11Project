#pragma once

#include "Client_Defines.h"
#include "Body_Wizard.h"

#include "Wizard_Candleabra.h"

BEGIN(Client)

class CBody_WizardCandleabra final : public CBody_Wizard
{
public:
	typedef struct tagBodyCandleabraDesc : public CBody_Wizard::DESC
	{

	}DESC;
private:
	CBody_WizardCandleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_WizardCandleabra(const CBody_WizardCandleabra& Prototype);
	virtual ~CBody_WizardCandleabra() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CWizard_Candleabra::STATES* m_pParentState = { nullptr };


private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBody_WizardCandleabra* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END