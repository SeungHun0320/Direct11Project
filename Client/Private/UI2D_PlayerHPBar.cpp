#include "UI2D_PlayerHPBar.h"

CUI2D_PlayerHPBar::CUI2D_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI2DContainerPart{pDevice, pContext}
{
}

CUI2D_PlayerHPBar::CUI2D_PlayerHPBar(const CUI2D_PlayerHPBar& Prototype)
	: CUI2DContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerHPBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CUI2D_PlayerHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerHPBar::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CUI2D_PlayerHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerHPBar::Render()
{
	return S_OK;
}

HRESULT CUI2D_PlayerHPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerHPBar::Ready_PartObjects()
{
	return S_OK;
}

CUI2D_PlayerHPBar* CUI2D_PlayerHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerHPBar* pInstance = new CUI2D_PlayerHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerHPBar::Clone(void* pArg)
{
	CUI2D_PlayerHPBar* pInstance = new CUI2D_PlayerHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerHPBar::Free()
{
	__super::Free();
}
