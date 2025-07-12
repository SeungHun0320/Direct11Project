#pragma once

#include "Effect_Part_Mesh.h"

class CEffect_Mesh_BossLager final : public CEffect_Part_Mesh
{
public:
	typedef struct tagEffectMeshBossLagerDesc : public CEffect_Part_Mesh::DESC
	{

	}DESC;

private:
	CEffect_Mesh_BossLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh_BossLager(const CEffect_Mesh_BossLager& Prototype);
	virtual ~CEffect_Mesh_BossLager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void TurnZ(_float fTimeDelta);
	void Scaling(const _float3& vScale);

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static  CEffect_Mesh_BossLager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

		//Prototype_Component_Model_Instance_BossLager
};

