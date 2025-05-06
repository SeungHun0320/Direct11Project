#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "BackGround.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	m_pBGM = m_pGameInstance->Get_Single_Sound("Logo");
	m_pBGM->Set_Volume(0.5f);
	m_pBGM->Play();

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::GAMEPLAY))))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CBackGround::DESC tDesc{};
	tDesc.fSizeX = g_iWinSizeX;
	tDesc.fSizeY = g_iWinSizeY;
	tDesc.fX = g_iWinSizeX * 0.5f;
	tDesc.fY = g_iWinSizeY * 0.5f;
	tDesc.eLevelID = LEVEL::LOGO;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}


CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();
	m_pBGM->Stop();
	Safe_Release(m_pBGM);
}
