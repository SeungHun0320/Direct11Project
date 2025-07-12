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

	for (_uint i = 0; i < SOCKET_END; i++)
		m_pSocketMatrix[i] = pDesc->pSocketMatrix[i];

	m_pParentWeaponType = pDesc->pParentWeaponType;
	m_pParentisAttacked = pDesc->pParentisAttacked;

	m_vColorStart = _float3(0.8f, 1.f, 1.f);   
	m_vColorEnd = _float3(0.7f, 1.f, 1.f);   
	m_fPowerAlpha = 0.2f;  
	m_fPowerColor = 1.f;   


	m_fAlpha = 1.f;

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
	if (WEAPON_TYPE::SWORD == *m_pParentWeaponType && (*m_pParentisAttacked))
		Update_Trail(m_pSocketMatrix[SWORD], m_pSocketMatrix[SWORD_TRAIL]);

	for (_uint i = 0; i < ENUM_CLASS(WEAPON_TYPE::WT_END); i++)
		m_pColliders[i]->Update(XMLoadFloat4x4(m_pParentMatrix));

	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	return LIFE::NONE;
}

void CWeapon_Player::Late_Update(_float fTimeDelta)
{
	if (WEAPON_TYPE::SWORD == *m_pParentWeaponType && (*m_pParentisAttacked))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

#ifdef _DEBUG
	for (auto& pCollider : m_pColliders)
		m_pGameInstance->Add_DebugComponent(pCollider);
#endif
}

HRESULT CWeapon_Player::Render()
{
	// 월드 / 뷰 / 프로젝션 바인딩
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_RawValue("g_vColorStart", &m_vColorStart, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColorEnd", &m_vColorEnd, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPowerAlpha", &m_fPowerAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPowerColor", &m_fPowerColor, sizeof(_float))))
		return E_FAIL;

	// 텍스쳐 & 알파
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	// 드로우
	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Player::Set_Active(WEAPON_TYPE eType, _bool isActive)
{
	m_pColliders[ENUM_CLASS(eType)]->Set_Active(isActive);
}

void CWeapon_Player::Update_Trail(const _float4x4* pBoneMatrix, const _float4x4* pTrailMatrix)
{
	_matrix		BoneMatrix = XMLoadFloat4x4(pBoneMatrix);
	_matrix		TrailMatrix = XMLoadFloat4x4(pTrailMatrix);

	for (_uint i = 0; i < 3; i++)
	{
		BoneMatrix.r[i] =  XMVector3Normalize(BoneMatrix.r[i]);
		TrailMatrix.r[i] = XMVector3Normalize(TrailMatrix.r[i]);
	}

	_vector vStart = BoneMatrix.r[3];
	_vector vEnd = TrailMatrix.r[3];

	_vector vCenter = (vStart + vEnd) * 0.5f;
	XMVECTOR vLook = XMVector3Normalize(vEnd - vStart);

	_float fHalfWidth = 0.5f;
	XMVECTOR vLeft = vCenter - vLook * fHalfWidth;
	XMVECTOR vRight = vCenter + vLook * fHalfWidth;

	XMFLOAT3 vLeft3, vRight3;
	XMStoreFloat3(&vLeft3, vLeft);
	XMStoreFloat3(&vRight3, vRight);

	m_pVIBufferCom->Update_Trail(vLeft3, vRight3);
	m_pVIBufferCom->Update_VertexBuffer();
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

	OBBDesc.vExtents = _float3(1.5f, 1.5f, 3.f);
	OBBDesc.vCenter = _float3(0.0f, OBBDesc.vExtents.y, OBBDesc.vExtents.z + 1.f);
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

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Trail"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
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

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
