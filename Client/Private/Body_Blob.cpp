#include "Body_Blob.h"

#include "GameInstance.h"

CBody_Blob::CBody_Blob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Blob::CBody_Blob(const CBody_Blob& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Blob::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Blob::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    /* 본, 애니메이션 얕복의 문제점 */
    /* 1. 서로 다른 애니메이션을 셋팅했음에도 같은 동작이 재생된다. : 뼈가 공유되기때문에. */
    /* 2. 같은 애니메이션을 셋했다면 재생속도가 빨라진다. : */
    return S_OK;
}

void CBody_Blob::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_Blob::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_Blob::Late_Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombindWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix));

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBody_Blob::Render()
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

_bool CBody_Blob::Play_Animation(_float fTimeDelta)
{
    return m_pModelCom->Play_Animation(fTimeDelta);
}

void CBody_Blob::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
    m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

HRESULT CBody_Blob::Ready_Components(void* pArg)
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Blob"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Blob::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombindWorldMatrix)))
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

CBody_Blob* CBody_Blob::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Blob* pInstance = new CBody_Blob(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Blob");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Blob::Clone(void* pArg)
{
    CBody_Blob* pInstance = new CBody_Blob(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Blob");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Blob::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
