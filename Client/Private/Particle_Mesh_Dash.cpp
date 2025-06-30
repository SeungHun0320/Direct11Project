#include "Particle_Mesh_Dash.h"
#include "GameInstance.h"

CParticle_Mesh_Dash::CParticle_Mesh_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CParticle_Mesh{pDevice, pContext}
{
}

CParticle_Mesh_Dash::CParticle_Mesh_Dash(const CParticle_Mesh_Dash& Prototype)
	:CParticle_Mesh(Prototype)
{
}

HRESULT CParticle_Mesh_Dash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Mesh_Dash::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentisNoStamina = pDesc->pParentisNoStamina;
	m_pParentisUseStamina = pDesc->pParentisUseStamina;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CParticle_Mesh_Dash::Priority_Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return;
}	   

LIFE CParticle_Mesh_Dash::Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return LIFE::NONE;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
		m_pModelCom->Shrink(i, fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix)
	);


	return LIFE::NONE;
}

void CParticle_Mesh_Dash::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CParticle_Mesh_Dash::Render()
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return S_OK;

	return __super::Render();
}

HRESULT CParticle_Mesh_Dash::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CParticle_Mesh_Dash* CParticle_Mesh_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Mesh_Dash* pInstance = new CParticle_Mesh_Dash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Mesh_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Mesh_Dash::Clone(void* pArg)
{
	CParticle_Mesh_Dash* pInstance = new CParticle_Mesh_Dash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Mesh_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CParticle_Mesh_Dash::Free()
{
	__super::Free();
}
