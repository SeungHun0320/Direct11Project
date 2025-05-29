#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

#include "Wizard_Sword.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPart_WizardSword final : public CPartObject
{
public:
	typedef struct tagPartSwordDesc : public CPartObject::DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		LEVEL eLevelID;
	}DESC;
private:
	CPart_WizardSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_WizardSword(const CPart_WizardSword& Prototype);
	virtual ~CPart_WizardSword() = default;

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
	CWizard_Sword::STATES* m_pParentState = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CPart_WizardSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END