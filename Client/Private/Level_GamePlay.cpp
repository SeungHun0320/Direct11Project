#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "Terrain.h"
#include "Camera_Free.h"
#include "Monster.h"

#include "Courtyard.h"

#include "Environment_Object.h"

CLevel_Courtyard::CLevel_Courtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Courtyard::Initialize()
{
	//if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Environmnet_Object(TEXT("Layer_EnvironObject"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Map(TEXT("Layer_Map"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Courtyard::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOGO))))
			return;
	}

}

HRESULT CLevel_Courtyard::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CTerrain::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::GAMEPLAY;
	tDesc.strName = TEXT("Terrain");
	tDesc.fSpeedPerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if(FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::GAMEPLAY;
	tDesc.fSensor = 0.1f;

	tDesc.vEye = _float3(0.f, 20.f, -15.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);
	tDesc.fFov = XMConvertToRadians(60.f);
	tDesc.fNear = 0.1f;
	tDesc.fFar = 5000.f;
	tDesc.fSpeedPerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Camera_Free");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CMonster::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::GAMEPLAY;
	tDesc.fSpeedPerSec = 20.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("SpiderTank");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Map(const _wstring& strLayerTag)
{
	CCourtyard::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::GAMEPLAY;
	tDesc.fRotationPerSec = XMConvertToRadians(0.f);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.strName = TEXT("Courtyard");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Environmnet_Object(const _wstring& strLayerTag)
{
	CEnvironment_Object::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::GAMEPLAY;
	tDesc.fRotationPerSec = 0.f;
	tDesc.fSpeedPerSec = 0.f;
	tDesc.strName = TEXT("Grass");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	tDesc.strName = TEXT("Bush");
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
	//	ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
	//	return E_FAIL;

	tDesc.strName = TEXT("CheckPoint");
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_") + tDesc.strName,
	//	ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
	//	return E_FAIL;

	return S_OK;
}

CLevel_Courtyard* CLevel_Courtyard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Courtyard* pInstance = new CLevel_Courtyard(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Courtyard");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Courtyard::Free()
{
	__super::Free();

}
