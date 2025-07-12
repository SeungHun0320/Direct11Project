#include "Effect_Part_AnimMesh.h"
#include "GameInstance.h"

CEffect_Part_AnimMesh::CEffect_Part_AnimMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CEffect_Part_AnimMesh::CEffect_Part_AnimMesh(const CEffect_Part_AnimMesh& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CEffect_Part_AnimMesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Part_AnimMesh::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentLevelID = pDesc->pParentLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Part_AnimMesh::Priority_Update(_float fTimeDelta)
{
}

LIFE CEffect_Part_AnimMesh::Update(_float fTimeDelta)
{
	m_fRatio += fTimeDelta;

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(&m_ParentMatrix));

	return LIFE::NONE;
}

void CEffect_Part_AnimMesh::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CEffect_Part_AnimMesh::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
		if (m_pModelCom->Get_MeshVisible(i))
			continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(4)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

void CEffect_Part_AnimMesh::Reset_Animation()
{
	m_pModelCom->Set_MeshVisible(0, true);
	m_pModelCom->Reset_Animation();
	m_fRatio = 0;
}

void CEffect_Part_AnimMesh::Set_MeshVisible(_bool isVisible)
{
	for(_uint i = 0; i < m_pModelCom->Get_NumMeshes(); i++)
		m_pModelCom->Set_MeshVisible(i, isVisible);
}

HRESULT CEffect_Part_AnimMesh::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);	

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS((*m_pParentLevelID)), pDesc->strPrototypeModelTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Part_AnimMesh::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDiscardRatio", &m_fRatio, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CEffect_Part_AnimMesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pMaskTextureCom);
}
