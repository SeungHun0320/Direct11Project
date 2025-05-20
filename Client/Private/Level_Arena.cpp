#include "Level_Arena.h"


CLevel_Arena::CLevel_Arena(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Arena::Initialize()
{
	return S_OK;
}

void CLevel_Arena::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Arena::Render()
{
	SetWindowText(g_hWnd, TEXT("상점레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Arena::Load_Map(const _wstring& strMapFileTag)
{
	return S_OK;
}


CLevel_Arena* CLevel_Arena::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Arena* pInstance = new CLevel_Arena(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Arena");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Arena::Free()
{
	__super::Free();
}


