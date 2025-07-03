#pragma once

#include "Effect_Mesh.h"

BEGIN(Client)

class CEffect_Mesh_Dodge final : public CEffect_Mesh
{
public:
	typedef struct tagParticleMeshDashDesc : public CEffect_Mesh::DESC
	{
		const _bool* pParentisUseStamina{ nullptr };
		const _bool* pParentisNoStamina{ nullptr };
		const _bool* pParentisRoll{ nullptr };
	}DESC;

private:
	CEffect_Mesh_Dodge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Mesh_Dodge(const CEffect_Mesh_Dodge& Prototype);
	virtual ~CEffect_Mesh_Dodge() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	const _bool* m_pParentisUseStamina = { nullptr };
	const _bool* m_pParentisNoStamina = { nullptr };
	const _bool* m_pParentisRoll = { nullptr };

private:
	_float m_fTimeAcc = {};

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CEffect_Mesh_Dodge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END