#include "Wizard_Support_AOE.h"
#include "GameInstance.h"

CWizard_Support_AOE::CWizard_Support_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster_Bullet_AOE{pDevice, pContext}
{
}

CWizard_Support_AOE::CWizard_Support_AOE(const CWizard_Support_AOE& Prototype)
    :CMonster_Bullet_AOE(Prototype)
{
}

HRESULT CWizard_Support_AOE::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWizard_Support_AOE::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION));

    m_fDeadTime = 5.f;

    return S_OK;
}

void CWizard_Support_AOE::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard_Support_AOE::Update(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_bDead)
        return LIFE::DEAD;


    if (m_fDeadTime <= m_fTimeAcc)
        m_bDead = true;

    return __super::Update(fTimeDelta);
}

void CWizard_Support_AOE::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Support_AOE::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif
    return S_OK;
}

HRESULT CWizard_Support_AOE::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(3.5f, 0.2f, 3.5f);
    AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::WIZARD_ATTACK);
    AABBDesc.pOwner = this;

    /* For.Com_Collider */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

CWizard_Support_AOE* CWizard_Support_AOE::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWizard_Support_AOE* pInstance = new CWizard_Support_AOE(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWizard_Support_AOE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWizard_Support_AOE::Clone(void* pArg)
{
	CWizard_Support_AOE* pInstance = new CWizard_Support_AOE(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWizard_Support_AOE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWizard_Support_AOE::Free()
{
    __super::Free();
}
