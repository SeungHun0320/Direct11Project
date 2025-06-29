#include "Particle_Mesh.h"
#include "GameInstance.h"

CParticle_Mesh::CParticle_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CParticle_Mesh::CParticle_Mesh(const CParticle_Mesh& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CParticle_Mesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Mesh::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentLevelID = pDesc->pParentLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CParticle_Mesh::Priority_Update(_float fTimeDelta)
{
}

LIFE CParticle_Mesh::Update(_float fTimeDelta)
{
	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
		m_pModelCom->Shrink(i, fTimeDelta);

	_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
	_vector ParentPos = ParentMatrix.r[3];

	_matrix ParentNoRot = XMMatrixIdentity();
	ParentNoRot.r[3] = ParentPos;

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix)
	);

	return LIFE::NONE;
}

void CParticle_Mesh::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CParticle_Mesh::Render()
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

HRESULT CParticle_Mesh::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMeshParticleInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS((*m_pParentLevelID)), TEXT("Prototype_Component_Model_Particle_Instance_Dash"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Mesh::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CParticle_Mesh* CParticle_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Mesh* pInstance = new CParticle_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Mesh::Clone(void* pArg)
{
	CParticle_Mesh* pInstance = new CParticle_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
