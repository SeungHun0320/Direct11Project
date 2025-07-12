#include "Effect_Mesh_BossLager.h"
#include "GameInstance.h"

CEffect_Mesh_BossLager::CEffect_Mesh_BossLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CEffect_Part_Mesh{pDevice, pContext}
{
}

CEffect_Mesh_BossLager::CEffect_Mesh_BossLager(const CEffect_Mesh_BossLager& Prototype)
    :CEffect_Part_Mesh(Prototype)
{
}

HRESULT CEffect_Mesh_BossLager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Mesh_BossLager::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(180.f), XMConvertToRadians(0.f));
    m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_State(STATE::POSITION) + XMVectorSet(0.f, 0.f, 4.f, 0.f));

    return S_OK;
}

void CEffect_Mesh_BossLager::Priority_Update(_float fTimeDelta)
{
}

LIFE CEffect_Mesh_BossLager::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix));

	return LIFE::NONE;
}

void CEffect_Mesh_BossLager::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_DEPTH_NONLIGHT, this);
}

HRESULT CEffect_Mesh_BossLager::Render()
{
    return __super::Render();
}

void CEffect_Mesh_BossLager::TurnZ(_float fTimeDelta)
{
    m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta);
}

void CEffect_Mesh_BossLager::Scaling(const _float3& vScale)
{
    m_pTransformCom->Scaling(vScale);
}

HRESULT CEffect_Mesh_BossLager::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

	return S_OK;
}

CEffect_Mesh_BossLager* CEffect_Mesh_BossLager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Mesh_BossLager* pInstance = new CEffect_Mesh_BossLager(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEffect_Mesh_BossLager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_Mesh_BossLager::Clone(void* pArg)
{
    CEffect_Mesh_BossLager* pInstance = new CEffect_Mesh_BossLager(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEffect_Mesh_BossLager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_Mesh_BossLager::Free()
{
	__super::Free();
}
