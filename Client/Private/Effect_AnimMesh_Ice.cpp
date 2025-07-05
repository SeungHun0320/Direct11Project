#include "Effect_AnimMesh_Ice.h"
#include "GameInstance.h"

CEffect_AnimMesh_Ice::CEffect_AnimMesh_Ice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect_AnimMesh{ pDevice, pContext }
{
}

CEffect_AnimMesh_Ice::CEffect_AnimMesh_Ice(const CEffect_AnimMesh_Ice& Prototype)
	: CEffect_AnimMesh(Prototype)
{
}

HRESULT CEffect_AnimMesh_Ice::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_AnimMesh_Ice::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;  

    return S_OK;
}

void CEffect_AnimMesh_Ice::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_AnimMesh_Ice::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CEffect_AnimMesh_Ice::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_AnimMesh_Ice::Render()
{
    return __super::Render();
}

HRESULT CEffect_AnimMesh_Ice::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

    return E_NOTIMPL;
}

CEffect_AnimMesh_Ice* CEffect_AnimMesh_Ice::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_AnimMesh_Ice* pInstance = new CEffect_AnimMesh_Ice(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_AnimMesh_Ice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_AnimMesh_Ice::Clone(void* pArg)
{
	CEffect_AnimMesh_Ice* pInstance = new CEffect_AnimMesh_Ice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_AnimMesh_Ice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_AnimMesh_Ice::Free()
{
	__super::Free();
}
