#include "Bullet_SpiderTankOrb.h"

#include "GameInstance.h"

CBullet_SpiderTankOrb::CBullet_SpiderTankOrb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet_Monster { pDevice, pContext }
{
}

CBullet_SpiderTankOrb::CBullet_SpiderTankOrb(const CBullet_SpiderTankOrb& Prototype)
	: CBullet_Monster( Prototype )
{
}

HRESULT CBullet_SpiderTankOrb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_SpiderTankOrb::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_vDir = pDesc->vDir;
	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pColliderCom->Set_Active(false);

	m_fDeadTime = 3.f;
	m_fAttack = 5.f;
	m_fStaggerValue = 10.f;

	return S_OK;
}

void CBullet_SpiderTankOrb::Priority_Update(_float fTimeDelta)
{
	
}

LIFE CBullet_SpiderTankOrb::Update(_float fTimeDelta)
{
	if (!m_bGrounded)
	{
		// 위치 적용
		m_vVelocity.y += -9.8f * fTimeDelta * 2;

		_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
		vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + m_vVelocity.y * fTimeDelta);

		m_pTransformCom->Set_State(STATE::POSITION, vPos);
		m_pTransformCom->Go_Dir(XMVector3Normalize(XMLoadFloat3(&m_vDir)), fTimeDelta);

		if (XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION)) <= XMVectorGetY(m_pTargetTransform->Get_State(STATE::POSITION)) + 1.f)
		{
			m_pColliderCom->Set_Active();
			m_bGrounded = true;
		}
	}
	else
	{
		m_pTransformCom->LookAtLerpEx(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 3.f);
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	return __super::Update(fTimeDelta);
}

void CBullet_SpiderTankOrb::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBullet_SpiderTankOrb::Render()
{
	return  __super::Render();
}

void CBullet_SpiderTankOrb::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	switch (eColliderID)
	{
	case COLLIDER_ID::PLAYER:
		Set_Dead(true);
		break;

	case COLLIDER_ID::SPIDERTANK_HEAD:
		Set_Dead(true);
		break;
		
	case COLLIDER_ID::SPIDERTANK_WEAK:
		Set_Dead(true);
		break;
	}

}

HRESULT CBullet_SpiderTankOrb::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::DESC	ColDesc{};
	_float3 vScale = m_pTransformCom->Get_Scaled();

	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.fRadius = vScale.x;
	ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET);
	ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_BULLET);
	ColDesc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
		return E_FAIL;

	return S_OK;
}

CBullet_SpiderTankOrb* CBullet_SpiderTankOrb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_SpiderTankOrb* pInstance = new CBullet_SpiderTankOrb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_SpiderTankOrb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_SpiderTankOrb::Clone(void* pArg)
{
	CBullet_SpiderTankOrb* pInstance = new CBullet_SpiderTankOrb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_SpiderTankOrb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_SpiderTankOrb::Free()
{
	__super::Free();

}
