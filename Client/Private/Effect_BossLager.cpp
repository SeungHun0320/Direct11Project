#include "Effect_BossLager.h"
#include "GameInstance.h"

#include "Effect_Mesh_BossLager.h"

CEffect_BossLager::CEffect_BossLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffectContainerPart{pDevice, pContext}
{
}

CEffect_BossLager::CEffect_BossLager(const CEffect_BossLager& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_BossLager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_BossLager::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParnetisUseLager = pDesc->pParnetisUseLager;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_isActive = true;

	return S_OK;
}

void CEffect_BossLager::Priority_Update(_float fTimeDelta)
{
	if (!(*m_pParnetisUseLager))
		return;

	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_BossLager::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (!(*m_pParnetisUseLager))
	{
		Set_MeshVisible(PART_LAGER, true);
		return LIFE::NONE;
	}


	Set_MeshVisible(PART_LAGER, false);
	TurnZ(PART_LAGER, fTimeDelta);

	return LIFE::NONE;
}

void CEffect_BossLager::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParnetisUseLager))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_BossLager::Render()
{
	return S_OK;
}

void CEffect_BossLager::Set_MeshVisible(_uint iPart, _bool isVisible)
{
	if (CEffect_Part_Mesh* pPart = dynamic_cast<CEffect_Part_Mesh*>(m_PartObjects[iPart]))
	{
		pPart->Set_MeshVisible(0, isVisible);
	}
}

void CEffect_BossLager::TurnZ(_uint iPart, _float fTimeDelta)
{
	if (CEffect_Mesh_BossLager* pPart = dynamic_cast<CEffect_Mesh_BossLager*>(m_PartObjects[iPart]))
	{
		pPart->TurnZ(fTimeDelta);
	}
}

void CEffect_BossLager::Scaling(_uint iPart, const _float3& vScale)
{
	if (CEffect_Mesh_BossLager* pPart = dynamic_cast<CEffect_Mesh_BossLager*>(m_PartObjects[iPart]))
	{
		pPart->Scaling(vScale);
	}
}

HRESULT CEffect_BossLager::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_BossLager::Ready_PartObjects()
{
	CEffect_Mesh_BossLager::DESC LagerDesc{};
	LagerDesc.pParentLevelID = m_pLevelID;
	LagerDesc.pParentMatrix = m_pParentMatrix;
	LagerDesc.strEffectModelTag = TEXT("Prototype_Component_Model_Instance_BossLager");
	LagerDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(PART_LAGER, ENUM_CLASS((*m_pLevelID)), TEXT("Prototype_GameObject_Effect_Mesh_BossLager"), &LagerDesc)))
		return E_FAIL;

	return S_OK;
}

CEffect_BossLager* CEffect_BossLager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_BossLager* pInstance = new CEffect_BossLager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_BossLager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_BossLager::Clone(void* pArg)
{
	CEffect_BossLager* pInstance = new CEffect_BossLager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_BossLager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_BossLager::Free()
{
	__super::Free();
}
