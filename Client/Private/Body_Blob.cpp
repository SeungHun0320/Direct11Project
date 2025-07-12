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

const _float4x4* CBody_Blob::Get_SocketMatrix(const _string& strBoneName)
{
    return m_pModelCom->Get_BoneMatrix(strBoneName);
}

HRESULT CBody_Blob::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Blob::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;
    m_pParentMatrix = pDesc->pParentMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_pColliderCom[ATTACK]->Set_Active(false);

    return S_OK;
}

void CBody_Blob::Priority_Update(_float fTimeDelta)
{

}

LIFE CBody_Blob::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    for(auto& pCollider : m_pColliderCom)
        pCollider->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

    return LIFE::NONE;
}

void CBody_Blob::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_SHADOW, this);

#ifdef _DEBUG
    for (auto& pCollider : m_pColliderCom)
        m_pGameInstance->Add_DebugComponent(pCollider);
#endif
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

HRESULT CBody_Blob::Render_Shadow()
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

_bool CBody_Blob::Play_Animation(_float fTimeDelta)
{
    return m_pModelCom->Play_Animation(fTimeDelta);
}

void CBody_Blob::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
    m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CBody_Blob::Set_TrackPosition(_float fTrackPosition)
{
    m_pModelCom->Set_CurrnetTrackPosition(fTrackPosition);
}

void CBody_Blob::Set_Active(_bool isActive)
{
    m_pColliderCom[ATTACK]->Set_Active(isActive);
}

CCollider* CBody_Blob::Get_Collider(_uint iColliderIndex) const
{
    return m_pColliderCom[iColliderIndex];
}

HRESULT CBody_Blob::Ready_Components(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Blob"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(0.8f, 0.8f, 0.8f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BLOB);
    AABBDesc.pOwner = pDesc->pOwner;

    /* For.Com_Collider_Body */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider_Body"), reinterpret_cast<CComponent**>(&m_pColliderCom[BODY]), &AABBDesc)))
        return E_FAIL;

    CBounding_OBB::DESC	OBBDesc{};
    OBBDesc.vExtents = _float3(1.f, 1.4f, 1.f);
    OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, OBBDesc.vExtents.z + OBBDesc.vExtents.z);
    OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK);
    OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BLOB_ATTACK);
    OBBDesc.pOwner = pDesc->pOwner;

    /* For.Com_Collider_Attack */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_Attack"), reinterpret_cast<CComponent**>(&m_pColliderCom[ATTACK]), &OBBDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Blob::Bind_ShaderResources()
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

    for (auto& pCollider : m_pColliderCom)
        Safe_Release(pCollider);

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
