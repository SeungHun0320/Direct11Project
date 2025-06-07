#include "SpiderTank_Bullet.h"

#include "GameInstance.h"

CSpiderTank_Bullet::CSpiderTank_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster_Bullet{ pDevice, pContext }
{
}

CSpiderTank_Bullet::CSpiderTank_Bullet(const CSpiderTank_Bullet& Prototype)
	: CMonster_Bullet(Prototype)
{
}

HRESULT CSpiderTank_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderTank_Bullet::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_vDir = pDesc->vDir;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vDir), 1.f));

	return S_OK;
}

void CSpiderTank_Bullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CSpiderTank_Bullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	m_pTransformCom->Go_Straight(fTimeDelta);

	if (2.5f >= XMVectorGetX(XMVector3Length(m_pTransformCom->Get_State(STATE::POSITION) -
		m_pTargetTransform->Get_State(STATE::POSITION))))
		m_bDead = true;

	return __super::Update(fTimeDelta);
}

void CSpiderTank_Bullet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank_Bullet::Render()
{
	return  __super::Render();
}

HRESULT CSpiderTank_Bullet::Ready_Components(void* pArg)
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

CSpiderTank_Bullet* CSpiderTank_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank_Bullet* pInstance = new CSpiderTank_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank_Bullet::Clone(void* pArg)
{
	CSpiderTank_Bullet* pInstance = new CSpiderTank_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank_Bullet::Free()
{
	__super::Free();
}
