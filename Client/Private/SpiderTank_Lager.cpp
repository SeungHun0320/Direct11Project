#include "SpiderTank_Lager.h"

#include "GameInstance.h"

CSpiderTank_Lager::CSpiderTank_Lager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster_Bullet{pDevice, pContext}
{
}

CSpiderTank_Lager::CSpiderTank_Lager(const CSpiderTank_Lager& Prototype)
    :CMonster_Bullet(Prototype)
{
}

HRESULT CSpiderTank_Lager::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSpiderTank_Lager::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentMatrix = pDesc->pParentMatrix;

	m_pTransformCom->Set_Matrix(XMLoadFloat4x4(m_pParentMatrix));

	m_fDeadTime = 1.f;
	m_fTimeAcc = 0.f;

    return S_OK;
}

void CSpiderTank_Lager::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CSpiderTank_Lager::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_bDead)
		return LIFE::DEAD;

	if (m_fDeadTime <= m_fTimeAcc)
		m_bDead = true;

	m_pColliderCom->Update(XMLoadFloat4x4(m_pParentMatrix));

    return LIFE::NONE;
}

void CSpiderTank_Lager::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank_Lager::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

    return S_OK;
}

HRESULT CSpiderTank_Lager::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::DESC	OBBDesc{};

	OBBDesc.vExtents = _float3(2.f, 1.f, 20.f);
	OBBDesc.vCenter = _float3(0.f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::BOSS_ATTACK);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::BOSS_LAGER);
	OBBDesc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

    return S_OK;
}

CSpiderTank_Lager* CSpiderTank_Lager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank_Lager* pInstance = new CSpiderTank_Lager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank_Lager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank_Lager::Clone(void* pArg)
{
	CSpiderTank_Lager* pInstance = new CSpiderTank_Lager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank_Lager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank_Lager::Free()
{
    __super::Free();
}
