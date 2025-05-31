#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

#include "Wizard_Support.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPart_WizardStaff final : public CPartObject
{
public:
	typedef struct tagPartStaffDesc : public CPartObject::DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		CWizard_Support::STATES* pParentState;
		LEVEL eLevelID;
	}DESC;
private:
	CPart_WizardStaff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_WizardStaff(const CPart_WizardStaff& Prototype);
	virtual ~CPart_WizardStaff() = default;

public:
	const _float4x4* Get_SocketMatrix(const _string& strBoneName);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	const _float4x4* m_pSocketMatrix = { nullptr };
	CWizard_Support::STATES* m_pParentState = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CPart_WizardStaff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END