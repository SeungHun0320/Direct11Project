#include "Level_Courtyard.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "Terrain.h"
#include "Courtyard.h"

#include "Camera_Free.h"

#include "Player.h"

#include "Monster.h"

#include "Chest.h"
#include "Item.h"

CLevel_Courtyard::CLevel_Courtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Courtyard::Initialize()
{
	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	//if (FAILED(Load_Map(TEXT("Courtyard.Map"))))
	//	return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

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

HRESULT CLevel_Courtyard::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//이 레벨의 플레이어 생성위치
	_float3 vInitPosition = { 0.f, 0.f, 0.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))
			->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vInitPosition), 1.f));
		static_cast<CPawn*>(pPlayer)->Set_Level(LEVEL::COURTYARD);
		return S_OK;
	}

	//없으면 새로 생성해서 넣어줌
	CPlayer::DESC tDesc{};
	tDesc.eLevelID = LEVEL::STATIC;
	tDesc.fSpeedPerSec = 20.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Player");

	
	tDesc.WorldMatrix = XMMatrixTranslation(vInitPosition.x, vInitPosition.y, vInitPosition.z);

	// 최초 게임 입장할때 어디에서 입장하던 스태틱에 생성해준다.
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Courtyard::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::COURTYARD;
	tDesc.fSensor = 0.1f;

	tDesc.vEye = _float3(0.f, 20.f, -15.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);
	tDesc.fFov = XMConvertToRadians(60.f);
	tDesc.fNear = 0.1f;
	tDesc.fFar = 5000.f;
	tDesc.fSpeedPerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Camera_Free");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CMonster::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::COURTYARD;
	tDesc.fSpeedPerSec = 20.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("SpiderTank");


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	CTerrain::DESC tDesc = {};
	tDesc.eLevelID = LEVEL::COURTYARD;
	tDesc.fSpeedPerSec = 0.f;
	tDesc.fRotationPerSec = 0.f;
	tDesc.strName = TEXT("Terrain");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Load_Map(const _wstring& strMapFileTag)
{
	ifstream LoadFile(TEXT("../bin/Resources/MapData/") + strMapFileTag, ios::binary);

	if (!LoadFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	_uint iNumMaps{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumMaps), sizeof(_uint));

	for (_uint i = 0; i < iNumMaps; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CMap::DESC tDesc{};
		tDesc.eLevelID = LEVEL::COURTYARD;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Map"), &tDesc)))
			return E_FAIL;
	}

	_uint iNumEnvironmentObjects{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumEnvironmentObjects), sizeof(_uint));
	for (_uint i = 0; i < iNumEnvironmentObjects; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CEnvironment_Object::DESC tDesc{};
		tDesc.eLevelID = LEVEL::COURTYARD;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Environment_Object"), &tDesc)))
			return E_FAIL;
	}

	_uint iNumItem{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumItem), sizeof(_uint));

	for (_uint i = 0; i < iNumItem; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CItem::DESC tDesc{};
		tDesc.eLevelID = LEVEL::COURTYARD;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Item"), &tDesc)))
			return E_FAIL;
	}

	_uint iNumChests{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumChests), sizeof(_uint));

	for (_uint i = 0; i < iNumChests; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CChest::DESC tDesc{};
		tDesc.eLevelID = LEVEL::COURTYARD;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Chest"), &tDesc)))
			return E_FAIL;
	}

	_uint iNumMonsters{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumMonsters), sizeof(_uint));

	for (_uint i = 0; i < iNumMonsters; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CMonster::DESC tDesc{};
		tDesc.eLevelID = LEVEL::COURTYARD;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Monster"), &tDesc)))
			return E_FAIL;
	}

	LoadFile.close();
	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

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
