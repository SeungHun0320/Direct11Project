#include "Level_Shop.h"


CLevel_Shop::CLevel_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Shop::Initialize()
{
	return S_OK;
}

void CLevel_Shop::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Shop::Render()
{
	SetWindowText(g_hWnd, TEXT("상점레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Shop::Load_Map(const _wstring& strMapFileTag)
{
	return S_OK;
}


CLevel_Shop* CLevel_Shop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Shop* pInstance = new CLevel_Shop(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Shop::Free()
{
	__super::Free();
}


