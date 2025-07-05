#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel_Particle_Instance;
END

BEGIN(Client)

class CEffect_Part_Mesh abstract : public CPartObject
{
public:
	typedef struct tagParticleMeshDesc : public CPartObject::DESC
	{
		_wstring strEffectModelTag{};
		LEVEL* pParentLevelID = { nullptr };
	}DESC;

protected:
	CEffect_Part_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Part_Mesh(const CEffect_Part_Mesh& Prototype);
	virtual ~CEffect_Part_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader*						  m_pShaderCom = { nullptr };
	CModel_Particle_Instance*         m_pModelCom = { nullptr };

protected:
	LEVEL* m_pParentLevelID = { nullptr };


protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END