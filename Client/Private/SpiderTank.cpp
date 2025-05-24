#include "SpiderTank.h"

#include "GameInstance.h"

CSpiderTank::CSpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss{pDevice, pContext}
{
}

CSpiderTank::CSpiderTank(const CSpiderTank& Prototype)
	: CBoss(Prototype)
{
}

HRESULT CSpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderTank::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 1. 서로 다른 애니메이션을 셋팅했음에도 같은 동작이 재생된다. : 뼈가 공유되기때문에. */
	/* 2. 같은 애니메이션을 셋했다면 재생속도가 빨라진다. : */
	m_pModelCom->Set_Animation(0, true);

	return S_OK;
}

void CSpiderTank::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CSpiderTank::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	if (KEY_DOWN(DIK_UP))
	{
		m_pModelCom->Change_Animation(++m_iSoonseo, true, 1.f, true);
	}
	if (KEY_DOWN(DIK_DOWN))
	{
		m_pModelCom->Change_Animation(--m_iSoonseo, true, 1.f, true);
	}

	return __super::Update(fTimeDelta);
}

void CSpiderTank::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank::Render()
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

HRESULT CSpiderTank::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_SpiderTank"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CSpiderTank* CSpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank* pInstance = new CSpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank::Clone(void* pArg)
{
	CSpiderTank* pInstance = new CSpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank::Free()
{
	__super::Free();
}
