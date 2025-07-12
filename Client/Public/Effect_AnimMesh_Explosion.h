#pragma once
#include "Effect_Obj_AnimMesh.h"

BEGIN(Client)

class CEffect_AnimMesh_Explosion final : public CEffect_Obj_AnimMesh
{
public:
	typedef struct tagEffectAnimMeshExplosionDesc : public CEffect_Obj_AnimMesh::DESC
	{

	}DESC;

private:
	CEffect_AnimMesh_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_AnimMesh_Explosion(const CEffect_AnimMesh_Explosion& Prototype);
	virtual ~CEffect_AnimMesh_Explosion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fTimeAcc = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CEffect_AnimMesh_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END