#include "Bullet_FireCracker.h"

#include "GameInstance.h"

CBullet_FireCracker::CBullet_FireCracker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBullet{pDevice, pContext}
{
}

CBullet_FireCracker::CBullet_FireCracker(const CBullet_FireCracker& Prototype)
    : CBullet(Prototype)
{
}

HRESULT CBullet_FireCracker::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBullet_FireCracker::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_vDir = pDesc->vDir;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_fDeadTime = 1.f;
    m_fAttack = 15.f;
    m_fStaggerValue = 10.f;

    m_pTransformCom->Scaling(0.75f, 0.75f, 0.75f);
    m_pTransformCom->LookDir(XMVectorSetW(XMLoadFloat3(&m_vDir), 0.f));

    XMStoreFloat3(&m_vVelocity, XMLoadFloat3(&m_vDir) * 15.f);
    m_vVelocity.y = 10.f;

    return S_OK;
}

void CBullet_FireCracker::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBullet_FireCracker::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fDeadTime <= m_fTimeAcc)
        Set_Dead(true);

    if (m_bDead)
    {
        m_fExplosionTime += fTimeDelta;

        if (0.5f <= m_fExplosionTime)
            return LIFE::DEAD;

        if (!m_isExplosion)
        {
            CBounding_Sphere::DESC	ColDesc{};

            ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
            ColDesc.fRadius = 3.5f;
            ColDesc.pOwner = this;
            ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::BULLET);
            ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BULLET_EXPLOSION);

            if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
                TEXT("Com_ExplosionCollider"), reinterpret_cast<CComponent**>(&m_pExplosionCollider), &ColDesc)))
                return LIFE::DEAD;

            m_isExplosion = true;
        }
    }

    if (!m_isExplosion)
    {
        m_vVelocity.y += m_fGravity * fTimeDelta;

        _vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
        _vector vVelocity = XMLoadFloat3(&m_vVelocity);
        _vector vNextPos = vPos + vVelocity * fTimeDelta;

        if (m_pNavigationCom->isMove(vNextPos))
        {
            _vector vGroundPos = m_pNavigationCom->SetUp_Height(vPos);
            _float fGroundY = XMVectorGetY(vGroundPos);
            _float fNextY = XMVectorGetY(vPos);

            // ¹Ù´Ú¿¡ ´ê¾ÒÀ» °æ¿ì Dead Ã³¸®
            if (fNextY <= fGroundY)
            {
                vNextPos = XMVectorSetY(vNextPos, fGroundY);
                Set_Dead(true);
            }
        }

        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(vNextPos, 1.f));
    }

    if (nullptr != m_pExplosionCollider)
        m_pExplosionCollider->Update(m_pTransformCom->Get_WorldMatrix());

    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    return  LIFE::NONE;
}

void CBullet_FireCracker::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
#ifdef _DEBUG
    if (nullptr != m_pExplosionCollider)
        m_pGameInstance->Add_DebugComponent(m_pExplosionCollider);
#endif
}

HRESULT CBullet_FireCracker::Render()
{
    return __super::Render();
}

void CBullet_FireCracker::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
    COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

    if (CI_MONSTER(eColliderID) || CI_ENVIRONMENT(eColliderID))
        Set_Dead(true);
}

HRESULT CBullet_FireCracker::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_Sphere::DESC	ColDesc{};

    ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
    ColDesc.fRadius = 0.75f;
    ColDesc.pOwner = this;
    ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::BULLET);
    ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BULLET_FIRE_CRACKER);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
        return E_FAIL;

    CNavigation::DESC tDesc{};
    XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

    if (LEVEL::TOOLS != m_eLevelID)
    {
        if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
            TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
            return E_FAIL;
    }

    return S_OK;

    return S_OK;
}

CBullet_FireCracker* CBullet_FireCracker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBullet_FireCracker* pInstance = new CBullet_FireCracker(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBullet_FireCracker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBullet_FireCracker::Clone(void* pArg)
{
    CBullet_FireCracker* pInstance = new CBullet_FireCracker(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBullet_FireCracker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBullet_FireCracker::Free()
{
    __super::Free();

    Safe_Release(m_pExplosionCollider);
    Safe_Release(m_pNavigationCom);
}
