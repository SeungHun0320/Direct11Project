#include "Effect_Part_Mesh.h"
#include "GameInstance.h"

CEffect_Part_Mesh::CEffect_Part_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CEffect_Part_Mesh::CEffect_Part_Mesh(const CEffect_Part_Mesh& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CEffect_Part_Mesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Part_Mesh::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentLevelID = pDesc->pParentLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Part_Mesh::Priority_Update(_float fTimeDelta)
{
}

LIFE CEffect_Part_Mesh::Update(_float fTimeDelta)
{
	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
		m_pModelCom->Spread(i, fTimeDelta);

	_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);

	ParentMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	ParentMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	ParentMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * ParentMatrix);

	return LIFE::NONE;
}

void CEffect_Part_Mesh::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CEffect_Part_Mesh::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Part_Mesh::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMeshParticleInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS((*m_pParentLevelID)), pDesc->strEffectModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Part_Mesh::Bind_ShaderResources()
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

void CEffect_Part_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
