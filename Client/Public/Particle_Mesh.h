#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel_Particle_Instance;
END

BEGIN(Client)

class CParticle_Mesh final : public CPartObject
{
public:
	typedef struct tagParticleMeshDesc : public CPartObject::DESC
	{
		_wstring strParticleBufferTag{};
		_wstring strParticleTextureTag{};
		LEVEL* pParentLevelID = { nullptr };
	}DESC;

private:
	CParticle_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Mesh(const CParticle_Mesh& Prototype);
	virtual ~CParticle_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*						  m_pShaderCom = { nullptr };
	CModel_Particle_Instance*         m_pModelCom = { nullptr };

private:
	LEVEL* m_pParentLevelID = { nullptr };


private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END