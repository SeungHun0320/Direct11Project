#include "Chest.h"
#include "GameInstance.h"

CChest::CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{ pDevice, pContext }
{
}

CChest::CChest(const CChest& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CChest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChest::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(0, true);

	return S_OK;
}

void CChest::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CChest::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	m_pModelCom->Play_Animation(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CChest::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CChest::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
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

HRESULT CChest::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototpye_Component_Model_Chest"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CChest* CChest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChest* pInstance = new CChest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChest::Clone(void* pArg)
{
	CChest* pInstance = new CChest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChest::Free()
{
	__super::Free();
}
