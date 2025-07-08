#pragma once

#include "Effect_AnimMesh.h"



BEGIN(Client)

class CEffect_AnimMesh_Ice final : public CEffect_AnimMesh
{
public:
	typedef struct tagEffectAnimMeshIceDesc : public CEffect_AnimMesh::DESC
	{

	}DESC;	

protected:
	CEffect_AnimMesh_Ice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_AnimMesh_Ice(const CEffect_AnimMesh_Ice& Prototype);
	virtual ~CEffect_AnimMesh_Ice() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg) override;
	
public:
	static CEffect_AnimMesh_Ice* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END