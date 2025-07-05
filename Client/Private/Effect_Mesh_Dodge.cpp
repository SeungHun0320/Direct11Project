#include "Effect_Mesh_Dodge.h"
#include "GameInstance.h"

CEffect_Mesh_Dodge::CEffect_Mesh_Dodge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect_Part_Mesh{pDevice, pContext}
{
}

CEffect_Mesh_Dodge::CEffect_Mesh_Dodge(const CEffect_Mesh_Dodge& Prototype)
	:CEffect_Part_Mesh(Prototype)
{
}

HRESULT CEffect_Mesh_Dodge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Mesh_Dodge::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentisNoStamina = pDesc->pParentisNoStamina;
	m_pParentisUseStamina = pDesc->pParentisUseStamina;
	m_pParentisRoll = pDesc->pParentisRoll;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Mesh_Dodge::Priority_Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return;
}	   

LIFE CEffect_Mesh_Dodge::Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return LIFE::NONE;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	if (!(*m_pParentisRoll))
	{
		for (_uint i = 0; i < iNumMesh; i++)
			m_pModelCom->Reset(i);
	}
	else
	{
		m_fTimeAcc += fTimeDelta;

		if (0.03f <= m_fTimeAcc)
		{
			for (_uint i = 0; i < iNumMesh; i++)
			{
				_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
				m_pModelCom->MoveTrail(i, XMVectorSetW(ParentMatrix.r[3], 0.f), fTimeDelta);
			}

			XMStoreFloat4x4(&m_CombinedWorldMatrix,
				XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()));

			m_fTimeAcc = 0.f;
		}

		for (_uint i = 0; i < iNumMesh; i++)
			m_pModelCom->Shrink(i, fTimeDelta);
	}


	return LIFE::NONE;
}

void CEffect_Mesh_Dodge::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Mesh_Dodge::Render()
{
	if (!(*m_pParentisUseStamina) || (*m_pParentisNoStamina))
		return S_OK;

	return __super::Render();
}

HRESULT CEffect_Mesh_Dodge::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CEffect_Mesh_Dodge* CEffect_Mesh_Dodge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Mesh_Dodge* pInstance = new CEffect_Mesh_Dodge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Mesh_Dodge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Mesh_Dodge::Clone(void* pArg)
{
	CEffect_Mesh_Dodge* pInstance = new CEffect_Mesh_Dodge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Mesh_Dodge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh_Dodge::Free()
{
	__super::Free();
}
