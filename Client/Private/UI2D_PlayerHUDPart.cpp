#include "UI2D_PlayerHUDPart.h"

CUI2D_PlayerHUDPart::CUI2D_PlayerHUDPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI2DContainerPart{pDevice, pContext}
{
}

CUI2D_PlayerHUDPart::CUI2D_PlayerHUDPart(const CUI2D_PlayerHUDPart& Prototype)
	: CUI2DContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerHUDPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerHUDPart::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CUI2D_PlayerHUDPart::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerHUDPart::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CUI2D_PlayerHUDPart::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerHUDPart::Render()
{
	return S_OK;
}

HRESULT CUI2D_PlayerHUDPart::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerHUDPart::Ready_PartObjects()
{
	return S_OK;
}

CUI2D_PlayerHUDPart* CUI2D_PlayerHUDPart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerHUDPart* pInstance = new CUI2D_PlayerHUDPart(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerHUDPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerHUDPart::Clone(void* pArg)
{
	CUI2D_PlayerHUDPart* pInstance = new CUI2D_PlayerHUDPart(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerHUDPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerHUDPart::Free()
{
	__super::Free();
}
