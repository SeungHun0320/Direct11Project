#include "SpiderTank_Orb.h"

#include "GameInstance.h"

CSpiderTank_Orb::CSpiderTank_Orb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster_Bullet { pDevice, pContext }
{
}

CSpiderTank_Orb::CSpiderTank_Orb(const CSpiderTank_Orb& Prototype)
	: CMonster_Bullet( Prototype )
{
}

HRESULT CSpiderTank_Orb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderTank_Orb::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_vDir = pDesc->vDir;
	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pColliderCom->Set_Active(false);

	return S_OK;
}

void CSpiderTank_Orb::Priority_Update(_float fTimeDelta)
{
	
}

LIFE CSpiderTank_Orb::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

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

void CSpiderTank_Orb::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank_Orb::Render()
{
	return  __super::Render();
}

HRESULT CSpiderTank_Orb::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_SpiderTankOrb"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::DESC	ColDesc{};
	_float3 vScale = m_pTransformCom->Get_Scaled();

	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.fRadius = vScale.x;
	ColDesc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
		return E_FAIL;

	return S_OK;
}

CSpiderTank_Orb* CSpiderTank_Orb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank_Orb* pInstance = new CSpiderTank_Orb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank_Orb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank_Orb::Clone(void* pArg)
{
	CSpiderTank_Orb* pInstance = new CSpiderTank_Orb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank_Orb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank_Orb::Free()
{
	__super::Free();

}
