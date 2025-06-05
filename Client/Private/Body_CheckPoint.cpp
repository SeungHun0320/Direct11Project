#include "Body_CheckPoint.h"

#include "GameInstance.h"

CBody_CheckPoint::CBody_CheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Environment_Object{ pDevice, pContext }
{
}

CBody_CheckPoint::CBody_CheckPoint(const CBody_CheckPoint& Prototype)
    : CBody_Environment_Object(Prototype)
{
}

HRESULT CBody_CheckPoint::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_CheckPoint::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_CheckPoint::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_CheckPoint::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CBody_CheckPoint::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_CheckPoint::Render()
{
    return __super::Render();
}

HRESULT CBody_CheckPoint::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_CheckPoint"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(3.5f, 3.5f, 3.5f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::CHECKPOINT);
    AABBDesc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

void CBody_CheckPoint::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
    cout << "체크포인트" << endl;
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
