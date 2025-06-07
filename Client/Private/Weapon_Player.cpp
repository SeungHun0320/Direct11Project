#include "Weapon_Player.h"

#include "GameInstance.h"

CWeapon_Player::CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPart_Player {pDevice, pContext}
{
}

CWeapon_Player::CWeapon_Player(const CWeapon_Player& Prototype)
	: CPart_Player(Prototype)
{
}

HRESULT CWeapon_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Player::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	for (auto& pCollider : m_pColliders)
		pCollider->Set_Active(false);

	return S_OK;
}

void CWeapon_Player::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWeapon_Player::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	for (_uint i = 0; i < ENUM_CLASS(WEAPON_TYPE::WT_END); i++)
		m_pColliders[i]->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

	return LIFE::NONE;
}

void CWeapon_Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWeapon_Player::Render()
{
#ifdef _DEBUG

	for (auto& pCollider : m_pColliders)
		pCollider->Render();

#endif // _DEBUG

	return S_OK;
}

void CWeapon_Player::Set_Active(WEAPON_TYPE eType, _bool isActive)
{
	m_pColliders[ENUM_CLASS(eType)]->Set_Active(isActive);
}

HRESULT CWeapon_Player::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Collider */
	CBounding_OBB::DESC	OBBDesc{};
	OBBDesc.vExtents = _float3(1.1f, 1.f, 1.3f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
	OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::WEAPON);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::STICK);
	OBBDesc.pOwner = pDesc->pOwner;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Stick"), reinterpret_cast<CComponent**>(&m_pColliders[ENUM_CLASS(WEAPON_TYPE::STICK)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.vExtents = _float3(1.7f, 1.f, 1.5f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SWORD);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Sword"), reinterpret_cast<CComponent**>(&m_pColliders[ENUM_CLASS(WEAPON_TYPE::SWORD)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.vExtents = _float3(1.5f, 1.5f, 2.5f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::DAGGER);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Dagger"), reinterpret_cast<CComponent**>(&m_pColliders[ENUM_CLASS(WEAPON_TYPE::DAGGER)]), &OBBDesc)))
		return E_FAIL;

	OBBDesc.vExtents = _float3(1.5f, 1.2f, 0.5f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, 1.f);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SHILED);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Shiled"), reinterpret_cast<CComponent**>(&m_pColliders[ENUM_CLASS(WEAPON_TYPE::SHILED)]), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CWeapon_Player* CWeapon_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Player* pInstance = new CWeapon_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Player::Clone(void* pArg)
{
	CWeapon_Player* pInstance = new CWeapon_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Player::Free()
{
	__super::Free();

	for (auto& Collider : m_pColliders)
		Safe_Release(Collider);
}
