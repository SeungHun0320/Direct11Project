#pragma once

#include "Client_Defines.h"
#include "Wizard_Weapon.h"

#include "Wizard_Sword.h"

BEGIN(Client)

class CPart_WizardCandleabra final : public CWizard_Weapon
{
public:
	typedef struct tagPartCandleabraDesc : public CWizard_Weapon::DESC
	{

	}DESC;
private:
	CPart_WizardCandleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_WizardCandleabra(const CPart_WizardCandleabra& Prototype);
	virtual ~CPart_WizardCandleabra() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Set_Active(_bool isActive) override;

private:
	CWizard_Sword::STATES* m_pParentState = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);

public:
	static CPart_WizardCandleabra* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END