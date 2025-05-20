#include "Level_Main.h"


CLevel_Main::CLevel_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Main::Initialize()
{
	return S_OK;
}

void CLevel_Main::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Main::Render()
{
	SetWindowText(g_hWnd, TEXT("상점레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Main::Load_Map(const _wstring& strMapFileTag)
{
	return S_OK;
}


CLevel_Main* CLevel_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Main* pInstance = new CLevel_Main(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Main");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Main::Free()
{
	__super::Free();
}


