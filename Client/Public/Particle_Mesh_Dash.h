#pragma once

#include "Particle_Mesh.h"

BEGIN(Client)

class CParticle_Mesh_Dash final : public CParticle_Mesh
{
public:
	typedef struct tagParticleMeshDashDesc : public CParticle_Mesh::DESC
	{
		const _bool* pParentisUseStamina{ nullptr };
		const _bool* pParentisNoStamina{ nullptr };
	}DESC;

private:
	CParticle_Mesh_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Mesh_Dash(const CParticle_Mesh_Dash& Prototype);
	virtual ~CParticle_Mesh_Dash() = default;

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

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CParticle_Mesh_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END