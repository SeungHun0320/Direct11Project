#include "Body_Grass.h"

#include "GameInstance.h"

CBody_Grass::CBody_Grass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Environment_Object{ pDevice, pContext }
{
}

CBody_Grass::CBody_Grass(const CBody_Grass& Prototype)
    : CBody_Environment_Object(Prototype)
{
}

HRESULT CBody_Grass::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Grass::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_Grass::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_Grass::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_Grass::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_Grass::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelInstanceCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        if (FAILED(m_pModelInstanceCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelInstanceCom->Render(i)))
            return E_FAIL;
    }

#ifdef _DEBUG

    m_pColliderCom->Render();

#endif

    return S_OK;

}

HRESULT CBody_Grass::Ready_Components(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMeshInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Instance_Grass"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelInstanceCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(0.4f, 0.8f, 0.4f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::GRASS);
    AABBDesc.pOwner = pDesc->pOwner;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

CBody_Grass* CBody_Grass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Grass* pInstance = new CBody_Grass(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Grass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Grass::Clone(void* pArg)
{
    CBody_Grass* pInstance = new CBody_Grass(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Grass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Grass::Free()
{
    __super::Free();

    Safe_Release(m_pModelInstanceCom);

    //while(m_pModelInstanceCom)
    //    Safe_Release(m_pModelInstanceCom);
}
