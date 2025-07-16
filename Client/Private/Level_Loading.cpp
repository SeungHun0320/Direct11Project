#include "Level_Loading.h"


#include "Level_Courtyard.h"
#include "Level_Arena.h"
#include "Level_Tools.h"
#include "Level_Shop.h"
#include "Level_Logo.h"
#include "Loader.h"

#include "GameInstance.h"

#include "LoadingScene.h"

#define CurLevel LEVEL::LOADING

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 로딩레벨 자체에 필요한 객체를 생성한다. */
	/* 배경, 로딩바, 버튼, font */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/* 로딩의 역할(다음레벨에 필요한 자원(Resource)(텍스쳐, 모델, 사운드 등등등 )을 생성하는)을 
	수행할 로더객체를 생성한다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;
	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	//if (KEY_DOWN(DIK_SPACE))
	{
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };

 			switch (m_eNextLevelID)
			{
			case LEVEL::LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::COURTYARD:
				pLevel = CLevel_Courtyard::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::ARENA:
				pLevel = CLevel_Arena::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::SHOP:
				pLevel = CLevel_Shop::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::TOOLS:
				pLevel = CLevel_Tools::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(m_eNextLevelID), pLevel)))
				return;
							
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	CLoadingScene::DESC tDesc{};
	tDesc.fSizeX = g_iWinSizeX;
	tDesc.fSizeY = g_iWinSizeY;
	tDesc.fX = g_iWinSizeX * 0.5f;
	tDesc.fY = g_iWinSizeY * 0.5f;
	tDesc.eLevelID = CurLevel;
	tDesc.strName = TEXT("LoadingScene");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
