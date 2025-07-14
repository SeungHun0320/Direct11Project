#include "Body_Merchant.h"
#include "GameInstance.h"

CBody_Merchant::CBody_Merchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{pDevice, pContext}
{
}

CBody_Merchant::CBody_Merchant(const CBody_Merchant& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Merchant::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Merchant::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);
    
    m_pParentLevelID = pDesc->pParentLevelID;
    m_pParentisFind = pDesc->pParentisFind;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_Merchant::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_Merchant::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    return LIFE::NONE;
}

void CBody_Merchant::Late_Update(_float fTimeDelta)
{
    if((*m_pParentisFind))
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBody_Merchant::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        //m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0);

        if (m_pModelCom->Get_MeshVisible(i))
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CBody_Merchant::Render_Shadow()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Light_ViewMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Light_ProjMatrix())))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(3)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

_bool CBody_Merchant::Play_Animation(_float fTimeDelta)
{
    return m_pModelCom->Play_Animation(fTimeDelta);
}

void CBody_Merchant::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
    m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

HRESULT CBody_Merchant::Ready_Components(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS((*m_pParentLevelID)), TEXT("Prototype_Component_Model_Merchant"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Merchant::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CBody_Merchant* CBody_Merchant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Merchant* pInstance = new CBody_Merchant(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Merchant");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Merchant::Clone(void* pArg)
{
    CBody_Merchant* pInstance = new CBody_Merchant(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Merchant");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Merchant::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
