#include "Effect_BossLager.h"
#include "GameInstance.h"

#include "Effect_Part_Mesh.h"

CEffect_BossLager::CEffect_BossLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffectContainerPart{pDevice, pContext}
{
}

CEffect_BossLager::CEffect_BossLager(const CEffect_BossLager& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_BossLager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_BossLager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_BossLager::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_BossLager::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEffect_BossLager::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_BossLager::Render()
{
	return S_OK;
}

void CEffect_BossLager::Set_MeshVisible(_uint iPart, _bool isVisible)
{
	if (CEffect_Part_Mesh* pPart = dynamic_cast<CEffect_Part_Mesh*>(m_PartObjects[iPart]))
	{
		pPart->Set_MeshVisible(0, isVisible);
	}
}

void CEffect_BossLager::TurnZ(_uint iPart, _float fTimeDelta)
{
}

HRESULT CEffect_BossLager::Ready_Components(void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CEffect_BossLager::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CEffect_BossLager* CEffect_BossLager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CEffect_BossLager::Clone(void* pArg)
{
	return nullptr;
}

void CEffect_BossLager::Free()
{
}
