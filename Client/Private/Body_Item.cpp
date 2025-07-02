#include "Body_Item.h"
#include "GameInstance.h"

CBody_Item::CBody_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{pDevice, pContext}
{
}

CBody_Item::CBody_Item(const CBody_Item& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CBody_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Item::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBody_Item::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_Item::Update(_float fTimeDelta)
{

	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

	return LIFE::NONE;
}

void CBody_Item::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CBody_Item::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBody_Item::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), pDesc->strPrototypeModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::DESC	AABBDesc{};
	AABBDesc.vExtents = pDesc->vExtents;
	AABBDesc.vCenter =  pDesc->vCenter;
	AABBDesc.iColliderGroupID = pDesc->iColliderGroupID;
	AABBDesc.iColliderID = pDesc->iColliderID;
	AABBDesc.pOwner = pDesc->pOwner;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Item::Bind_ShaderResources()
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

CBody_Item* CBody_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Item* pInstance = new CBody_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Item::Clone(void* pArg)
{
	CBody_Item* pInstance = new CBody_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Item::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
