#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel_Particle_Instance;
END

BEGIN(Client)

class CEffect_Mesh : public CPartObject
{
public:
	typedef struct tagParticleMeshDesc : public CPartObject::DESC
	{
		_wstring strParticleModelTag{};
		LEVEL* pParentLevelID = { nullptr };
	}DESC;

protected:
	CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh(const CEffect_Mesh& Prototype);
	virtual ~CEffect_Mesh() = default;

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
	static CEffect_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END