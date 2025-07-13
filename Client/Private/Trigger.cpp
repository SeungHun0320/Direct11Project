#include "Trigger.h"
#include "GameInstance.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{pDevice, pContext}
{
}

CTrigger::CTrigger(const CTrigger& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID    = pDesc->eLevelID;
    m_eColliderID = pDesc->eColliderID;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CTrigger::Priority_Update(_float fTimeDelta)
{
}

LIFE CTrigger::Update(_float fTimeDelta)
{
    if (m_bDead)
        return LIFE::DEAD;

    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    return LIFE::NONE;
}

void CTrigger::Late_Update(_float fTimeDelta)
{
#ifdef _DEBUG
    if (FAILED(m_pGameInstance->Add_DebugComponent(m_pColliderCom)))
        return;
#endif
}

HRESULT CTrigger::Render()
{
    return S_OK;
}

HRESULT CTrigger::Ready_Components(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    CBounding_Sphere::DESC	ColDesc{};
    ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColDesc.fRadius = 3.f;
    ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::TRIGGER);
    ColDesc.iColliderID = ENUM_CLASS(m_eColliderID);
    ColDesc.pOwner = this;

    /* For.Com_Collider_Head */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
        return E_FAIL;

    return S_OK;
}

void CTrigger::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
    COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

    //if (CI_PLAYER(eColliderID))
    //    Set_Dead(true);
}

CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTrigger* pInstance = new CTrigger(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTrigger");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
    CTrigger* pInstance = new CTrigger(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CTrigger");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTrigger::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
}
