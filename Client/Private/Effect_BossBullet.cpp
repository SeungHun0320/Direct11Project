#include "Effect_BossBullet.h"
#include "GameInstance.h"

#include "Effect_Mesh_BossMuzzle.h"

CEffect_BossBullet::CEffect_BossBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart {pDevice, pContext}
{
}

CEffect_BossBullet::CEffect_BossBullet(const CEffect_BossBullet& Prototype)
	: CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_BossBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_BossBullet::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentisShot = pDesc->pParentisShot;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_isActive = true;

	return S_OK;
}

void CEffect_BossBullet::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentisShot))
        return;

    __super::Priority_Update(fTimeDelta);
}

LIFE CEffect_BossBullet::Update(_float fTimeDelta)
{
    if (!(*m_pParentisShot))
    {
        TurnZ(PART_MUZZLE, fTimeDelta);
        Set_MeshVisible(PART_MUZZLE, false);
        return LIFE::NONE;
    }


	return __super::Update(fTimeDelta);
}

void CEffect_BossBullet::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentisShot))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_BossBullet::Render()
{
	return S_OK;
}

void CEffect_BossBullet::Set_MeshVisible(_uint iPart, _bool isVisible)
{
    if (CEffect_Part_Mesh* pPart = dynamic_cast<CEffect_Part_Mesh*>(m_PartObjects[iPart]))
    {
        pPart->Set_MeshVisible(0, isVisible);
    }
}

void CEffect_BossBullet::TurnZ(_uint iPart, _float fTimeDelta)
{
    if (CEffect_Mesh_BossMuzzle* pPart = dynamic_cast<CEffect_Mesh_BossMuzzle*>(m_PartObjects[iPart]))
    {
        pPart->TurnZ(fTimeDelta);
    }
}

HRESULT CEffect_BossBullet::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_BossBullet::Ready_PartObjects()
{
    CEffect_Mesh_BossMuzzle::DESC MuzzleDesc{};
    MuzzleDesc.pParentLevelID = m_pLevelID; 
    MuzzleDesc.pParentMatrix = m_pParentMatrix;
    MuzzleDesc.strEffectModelTag = TEXT("Prototype_Component_Model_Particle_Instance_BossMuzzle");
    MuzzleDesc.fRotationPerSec = 10.f;

    if (FAILED(__super::Add_PartObject(PART_MUZZLE, ENUM_CLASS((*m_pLevelID)), TEXT("Prototype_GameObject_Effect_BossMuzzle"), &MuzzleDesc)))
        return E_FAIL;

    return S_OK;
}

CEffect_BossBullet* CEffect_BossBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_BossBullet* pInstance = new CEffect_BossBullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEffect_BossBullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_BossBullet::Clone(void* pArg)
{
    CEffect_BossBullet* pInstance = new CEffect_BossBullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEffect_BossBullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_BossBullet::Free()
{
    __super::Free();
}