#pragma once

#include "Effect_Obj_Mesh.h"

BEGIN(Client)

class CEffect_Mesh_Smoke final : public CEffect_Obj_Mesh
{
public:
	typedef struct tagParticleMeshSmokeDesc : public CEffect_Obj_Mesh::DESC
	{
	}DESC;

private:
	CEffect_Mesh_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh_Smoke(const CEffect_Mesh_Smoke& Prototype);
	virtual ~CEffect_Mesh_Smoke() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fTimeAcc = {};

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CEffect_Mesh_Smoke* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END