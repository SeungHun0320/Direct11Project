#include "Body_CheckPoint.h"

#include "GameInstance.h"

CBody_CheckPoint::CBody_CheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_CheckPoint::CBody_CheckPoint(const CBody_CheckPoint& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_CheckPoint::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_CheckPoint::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;
    m_pParentMatrix = pDesc->pParentMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_CheckPoint::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_CheckPoint::Update(_float fTimeDelta)
{
    return LIFE::NONE;
}

void CBody_CheckPoint::Late_Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBody_CheckPoint::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CBody_CheckPoint::Ready_Components(void* pArg)
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_CheckPoint"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_CheckPoint::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CBody_CheckPoint* CBody_CheckPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_CheckPoint* pInstance = new CBody_CheckPoint(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_CheckPoint");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_CheckPoint::Clone(void* pArg)
{
    CBody_CheckPoint* pInstance = new CBody_CheckPoint(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_CheckPoint");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_CheckPoint::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
