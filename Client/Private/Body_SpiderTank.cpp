#include "Body_SpiderTank.h"

#include "GameInstance.h"

CBody_SpiderTank::CBody_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_SpiderTank::CBody_SpiderTank(const CBody_SpiderTank& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CBody_SpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_SpiderTank::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	
	m_pColMatrix[CSpiderTank::HEAD] = m_pModelCom->Get_BoneMatrix("head");
	m_pColMatrix[CSpiderTank::WEAK] = m_pModelCom->Get_BoneMatrix("powercell");
	m_pColMatrix[CSpiderTank::LEFT_ARM] = m_pModelCom->Get_BoneMatrix("arm_4_L_end");
	m_pColMatrix[CSpiderTank::RIGHT_ARM] = m_pModelCom->Get_BoneMatrix("arm_4_R_end");

	return S_OK;
}

void CBody_SpiderTank::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_SpiderTank::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));


	for(_uint i = 0; i < CSpiderTank::COL_END; i++)
		m_pColliderCom[i]->Update(XMLoadFloat4x4(m_pColMatrix[i]) * XMLoadFloat4x4(&m_CombinedWorldMatrix));

	return LIFE::NONE;
}

void CBody_SpiderTank::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	for (auto& pCollider : m_pColliderCom)
		m_pGameInstance->Add_DebugComponent(pCollider);
#endif
}

HRESULT CBody_SpiderTank::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
			return E_FAIL;

		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

_bool CBody_SpiderTank::Is_CurrentAnim(_uint iNextIndex)
{
	return m_pModelCom->Is_CurrentAnim(iNextIndex);
}

_bool CBody_SpiderTank::Play_Animation(_float fTimeDelta)
{
	return m_pModelCom->Play_Animation(fTimeDelta);
}

void CBody_SpiderTank::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CBody_SpiderTank::Set_TrackPosition(_float fTrackPosition)
{
	m_pModelCom->Set_CurrnetTrackPosition(fTrackPosition);
}

void CBody_SpiderTank::Set_TickPerSecond(_float fTickPerSecond)
{
	m_pModelCom->Set_NextTickPerSecond(fTickPerSecond);
}

void CBody_SpiderTank::Set_Active(CSpiderTank::COL_TYPE eType, _bool isActive)
{
	m_pColliderCom[eType]->Set_Active(isActive);
}

const _float4x4* CBody_SpiderTank::Get_BoneMatrix(const _string& strBoneName) const
{
	return m_pModelCom->Get_BoneMatrix(strBoneName);
}

HRESULT CBody_SpiderTank::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_SpiderTank"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	CBounding_Sphere::DESC	ColDesc{};
	ColDesc.vCenter = _float3(0.f, 0.f, -100.f);
	ColDesc.fRadius = 300.f;
	ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER);
	ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_HEAD);
	ColDesc.pOwner = pDesc->pOwner;

	/* For.Com_Collider_Head */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_pColliderCom[CSpiderTank::HEAD]), &ColDesc)))
		return E_FAIL;

	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.fRadius = 250.f;
	ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_WEAK);

	/* For.Com_Collider_Weak */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Weak"), reinterpret_cast<CComponent**>(&m_pColliderCom[CSpiderTank::WEAK]), &ColDesc)))
		return E_FAIL;

	CBounding_OBB::DESC OBBDesc{};
	OBBDesc.vExtents = _float3(300.f, 600.f, 300.f);
	OBBDesc.vCenter = _float3(0.f, -OBBDesc.vExtents.y, 0.f);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_ATTACK);
	OBBDesc.pOwner = pDesc->pOwner;

	/* For.Com_Collider_LeftArm */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_LeftArm"), reinterpret_cast<CComponent**>(&m_pColliderCom[CSpiderTank::LEFT_ARM]), &OBBDesc)))
		return E_FAIL;

	/* For.Com_Collider_RightArm */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_RightArm"), reinterpret_cast<CComponent**>(&m_pColliderCom[CSpiderTank::RIGHT_ARM]), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_SpiderTank::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CBody_SpiderTank* CBody_SpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_SpiderTank* pInstance = new CBody_SpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_SpiderTank::Clone(void* pArg)
{
	CBody_SpiderTank* pInstance = new CBody_SpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_SpiderTank::Free()
{
	__super::Free();

	for (_uint i = 0; i < CSpiderTank::COL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
