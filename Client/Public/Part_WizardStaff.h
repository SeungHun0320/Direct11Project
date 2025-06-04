#pragma once

#include "Client_Defines.h"
#include "Wizard_Weapon.h"

#include "Wizard_Support.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPart_WizardStaff final : public CWizard_Weapon
{
public:
	typedef struct tagPartStaffDesc : public CWizard_Weapon::DESC
	{
		CWizard_Support::STATES* pParentState;
	}DESC;
private:
	CPart_WizardStaff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_WizardStaff(const CPart_WizardStaff& Prototype);
	virtual ~CPart_WizardStaff() = default;

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
	HRESULT Ready_Components(void* pArg);

public:
	static CPart_WizardStaff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END