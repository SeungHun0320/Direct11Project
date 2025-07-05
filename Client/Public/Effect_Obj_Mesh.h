#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel_Particle_Instance;
END

BEGIN(Client)

class CEffect_Obj_Mesh abstract : public CGameObject
{
public:
	typedef struct tagParticleMeshDesc : public CGameObject::DESC
	{
		_wstring strEffectModelTag{};
		LEVEL eLevelID = {LEVEL::LEVEL_END};
	}DESC;

protected:
	CEffect_Obj_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Obj_Mesh(const CEffect_Obj_Mesh& Prototype);
	virtual ~CEffect_Obj_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel_Particle_Instance* m_pModelCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };


protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END