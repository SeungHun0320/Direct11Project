#pragma once

#include "Client_Defines.h"
#include "Wizard_Weapon.h"

#include "Wizard_Sword.h"

BEGIN(Client)

class CPart_WizardSword final : public CWizard_Weapon
{
public:
	typedef struct tagPartSwordDesc : public CWizard_Weapon::DESC
	{
		_float3 vScale{};

	}DESC;
private:
	CPart_WizardSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_WizardSword(const CPart_WizardSword& Prototype);
	virtual ~CPart_WizardSword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CWizard_Sword::STATES* m_pParentState = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CPart_WizardSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END