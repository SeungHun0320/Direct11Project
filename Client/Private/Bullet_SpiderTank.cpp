#include "Bullet_SpiderTank.h"

#include "GameInstance.h"

CBullet_SpiderTank::CBullet_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet_Monster{ pDevice, pContext }
{
}

CBullet_SpiderTank::CBullet_SpiderTank(const CBullet_SpiderTank& Prototype)
	: CBullet_Monster(Prototype)
{
}

HRESULT CBullet_SpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_SpiderTank::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_vDir = pDesc->vDir;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDeadTime = 5.f;
	m_fAttack = 2.5f;
	m_fStaggerValue = 0.f;

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vDir), 1.f));

	return S_OK;
}

void CBullet_SpiderTank::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBullet_SpiderTank::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CBullet_SpiderTank::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta); 
}

HRESULT CBullet_SpiderTank::Render()
{
	return  __super::Render();
}

void CBullet_SpiderTank::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	switch (eColliderID)
	{
	case COLLIDER_ID::PLAYER:
		Set_Dead(true);
		break;
	}

}

HRESULT CBullet_SpiderTank::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::DESC	ColDesc{};
	_float3 vScale = m_pTransformCom->Get_Scaled();

	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.fRadius = vScale.x;
	ColDesc.pOwner = this;
	ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET);
	ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_BULLET);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
		return E_FAIL;

	return S_OK;
}

CBullet_SpiderTank* CBullet_SpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_SpiderTank* pInstance = new CBullet_SpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_SpiderTank::Clone(void* pArg)
{
	CBullet_SpiderTank* pInstance = new CBullet_SpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_SpiderTank::Free()
{
	__super::Free();
}
