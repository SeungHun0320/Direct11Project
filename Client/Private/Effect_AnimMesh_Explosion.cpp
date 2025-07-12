#include "Effect_AnimMesh_Explosion.h"
#include "GameInstance.h"

CEffect_AnimMesh_Explosion::CEffect_AnimMesh_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect_Obj_AnimMesh{pDevice, pContext}
{
}

CEffect_AnimMesh_Explosion::CEffect_AnimMesh_Explosion(const CEffect_AnimMesh_Explosion& Prototype)
	: CEffect_Obj_AnimMesh(Prototype)
{
}

HRESULT CEffect_AnimMesh_Explosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_AnimMesh_Explosion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(0, false);

	return S_OK;
}

void CEffect_AnimMesh_Explosion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_AnimMesh_Explosion::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	m_fTimeAcc += fTimeDelta;

	if (m_pModelCom->Play_Animation(fTimeDelta) || 0.2f <= m_fTimeAcc)
		Set_Dead(true);

	return __super::Update(fTimeDelta);
}

void CEffect_AnimMesh_Explosion::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_AnimMesh_Explosion::Render()
{
	return __super::Render();
}

HRESULT CEffect_AnimMesh_Explosion::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

CEffect_AnimMesh_Explosion* CEffect_AnimMesh_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_AnimMesh_Explosion* pInstance = new CEffect_AnimMesh_Explosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_AnimMesh_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_AnimMesh_Explosion::Clone(void* pArg)
{
	CEffect_AnimMesh_Explosion* pInstance = new CEffect_AnimMesh_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_AnimMesh_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AnimMesh_Explosion::Free()
{
	__super::Free();
}
