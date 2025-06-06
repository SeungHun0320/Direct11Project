#include "Body_Bush.h"

#include "GameInstance.h"

CBody_Bush::CBody_Bush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Environment_Object{ pDevice, pContext }
{
}

CBody_Bush::CBody_Bush(const CBody_Bush& Prototype)
    : CBody_Environment_Object(Prototype)
{
}

HRESULT CBody_Bush::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Bush::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_Bush::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_Bush::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_Bush::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_Bush::Render()
{
    return __super::Render();
}

HRESULT CBody_Bush::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Bush"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(0.8f, 0.8f, 0.8f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BUSH);
    AABBDesc.pOwner = pDesc->pOwner;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

CBody_Bush* CBody_Bush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Bush* pInstance = new CBody_Bush(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Bush");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Bush::Clone(void* pArg)
{
    CBody_Bush* pInstance = new CBody_Bush(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Bush");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Bush::Free()
{
    __super::Free();
}
