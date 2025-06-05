#include "Body_Chest.h"

#include "GameInstance.h"

CBody_Chest::CBody_Chest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Environment_Object{ pDevice, pContext }
{
}

CBody_Chest::CBody_Chest(const CBody_Chest& Prototype)
    : CBody_Environment_Object(Prototype)
{
}

HRESULT CBody_Chest::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Chest::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_Chest::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_Chest::Update(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);

    return __super::Update(fTimeDelta);
}

void CBody_Chest::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_Chest::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

#ifdef _DEBUG

    m_pColliderCom->Render();

#endif

    return S_OK;
}

HRESULT CBody_Chest::Ready_Components(void* pArg)
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Chest"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(1.f, 1.f, 1.f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::CHEST);
    AABBDesc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

void CBody_Chest::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    cout << "»óÀÚ" << endl;
}


CBody_Chest* CBody_Chest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Chest* pInstance = new CBody_Chest(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Chest");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Chest::Clone(void* pArg)
{
    CBody_Chest* pInstance = new CBody_Chest(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Chest");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Chest::Free()
{
    __super::Free();
}
