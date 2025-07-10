#include "Effect_Mesh_BossMuzzle.h"
#include "GameInstance.h"

CEffect_Mesh_BossMuzzle::CEffect_Mesh_BossMuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CEffect_Part_Mesh{pDevice, pContext}
{
}

CEffect_Mesh_BossMuzzle::CEffect_Mesh_BossMuzzle(const CEffect_Mesh_BossMuzzle& Prototype)
    : CEffect_Part_Mesh(Prototype)
{
}

HRESULT CEffect_Mesh_BossMuzzle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Mesh_BossMuzzle::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, m_pTransformCom->Get_State(STATE::POSITION) + XMVectorSet(0.f, 0.f, 2.f, 0.f));
    m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(180.f), XMConvertToRadians(0.f));

    return S_OK;
}

void CEffect_Mesh_BossMuzzle::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Mesh_BossMuzzle::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix));

    return LIFE::NONE;
}

void CEffect_Mesh_BossMuzzle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_DEPTH_NONLIGHT, this);
}

HRESULT CEffect_Mesh_BossMuzzle::Render()
{
    return __super::Render();
}

void CEffect_Mesh_BossMuzzle::TurnZ(_float fTimeDelta)
{
    m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta);
}

HRESULT CEffect_Mesh_BossMuzzle::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

CEffect_Mesh_BossMuzzle* CEffect_Mesh_BossMuzzle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_Mesh_BossMuzzle* pInstance = new CEffect_Mesh_BossMuzzle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEffect_Mesh_BossMuzzle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_Mesh_BossMuzzle::Clone(void* pArg)
{
    CEffect_Mesh_BossMuzzle* pInstance = new CEffect_Mesh_BossMuzzle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEffect_Mesh_BossMuzzle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_Mesh_BossMuzzle::Free()
{
    __super::Free();
}
