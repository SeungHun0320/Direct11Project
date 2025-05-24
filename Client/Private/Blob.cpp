#include "Blob.h"

#include "GameInstance.h"

CBlob::CBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CBlob::CBlob(const CBlob& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CBlob::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlob::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 본, 애니메이션 얕복의 문제점 */
	/* 1. 서로 다른 애니메이션을 셋팅했음에도 같은 동작이 재생된다. : 뼈가 공유되기때문에. */
	/* 2. 같은 애니메이션을 셋했다면 재생속도가 빨라진다. : */
	m_pModelCom->Set_Animation(2, true);

	return S_OK;
}

void CBlob::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBlob::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, m_pTransformCom);

	return __super::Update(fTimeDelta);
}

void CBlob::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlob::Render()
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

HRESULT CBlob::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Blob"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CBlob* CBlob::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlob* pInstance = new CBlob(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlob");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlob::Clone(void* pArg)
{
	CBlob* pInstance = new CBlob(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlob");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlob::Free()
{
	__super::Free();
}
