#include "Effect_Mesh_Smoke.h"
#include "GameInstance.h"

CEffect_Mesh_Smoke::CEffect_Mesh_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect_Obj_Mesh{ pDevice, pContext }	
{
}

CEffect_Mesh_Smoke::CEffect_Mesh_Smoke(const CEffect_Mesh_Smoke& Prototype)
	:CEffect_Obj_Mesh(Prototype)
{
}

HRESULT CEffect_Mesh_Smoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Mesh_Smoke::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Mesh_Smoke::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);	
}

LIFE CEffect_Mesh_Smoke::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
		m_pModelCom->Spread(i, fTimeDelta);

	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > m_pModelCom->Get_MaxLifeTime(0))
	{
		m_fTimeAcc = 0.f;
		m_pModelCom->Reset(0);
		m_bDead = true;
	}

	return LIFE::NONE;
}

void CEffect_Mesh_Smoke::Late_Update(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Mesh_Smoke::Render()
{
	return __super::Render();
}

HRESULT CEffect_Mesh_Smoke::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CEffect_Mesh_Smoke* CEffect_Mesh_Smoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Mesh_Smoke* pInstance = new CEffect_Mesh_Smoke(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Mesh_Smoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Mesh_Smoke::Clone(void* pArg)
{
	CEffect_Mesh_Smoke* pInstance = new CEffect_Mesh_Smoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Mesh_Smoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh_Smoke::Free()
{
	__super::Free();
}
