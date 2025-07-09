#pragma once

#include "Effect_Part_Mesh.h"

class CEffect_Mesh_BossMuzzle final : public CEffect_Part_Mesh
{
public:
	typedef struct tagEffectBossMuzzleDesc : public CEffect_Part_Mesh::DESC
	{
	}DESC;

protected:
	CEffect_Mesh_BossMuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh_BossMuzzle(const CEffect_Mesh_BossMuzzle& Prototype);
	virtual ~CEffect_Mesh_BossMuzzle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void TurnZ(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static  CEffect_Mesh_BossMuzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

