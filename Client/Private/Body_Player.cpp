#include "Body_Player.h"

#include "GameInstance.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPart_Player {pDevice, pContext}
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPart_Player(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_Player::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_Player::Render()
{
    return __super::Render();
}

HRESULT CBody_Player::Ready_Components(void* pArg)
{    
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fox"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(0.6f, 1.2f, 0.6f);
    AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::PAWN);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::PLAYER);
    AABBDesc.pOwner = pDesc->pOwner;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();
}
