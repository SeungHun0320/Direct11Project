#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

class CWizard_Weapon abstract : public CPartObject
{
public:
	typedef struct tagWizardWeaponDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
		const _float4x4* pSocketMatrix = { nullptr };
	}DESC;

protected:
	CWizard_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWizard_Weapon(const CWizard_Weapon& Prototype);
	virtual ~CWizard_Weapon() = default;

public:
	const _float4x4* Get_SocketMatrix(const _string& strBoneName);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CCollider*  m_pColliderCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

protected:
	const _float4x4* m_pSocketMatrix = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

