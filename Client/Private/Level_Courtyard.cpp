#include "Level_Courtyard.h"
#include "Level_Loading.h"

#include "Camera_Free.h"
#include "Camera_TPS.h"

#include "Terrain.h"
#include "Sky.h"
#include "Courtyard.h"

#include "Player.h"


#include "SpiderTank.h"

#include "Chest.h"
#include "Item.h"
#include "Monster.h"

#define CurLevel LEVEL::COURTYARD

CLevel_Courtyard::CLevel_Courtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Courtyard::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("Courtyard.Map"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	m_pGameInstance->Set_CameraMode(ENUM_CLASS(CurLevel), TEXT("Camera_TPS"), ENUM_CLASS(CAM_MODE::TPS));

	m_pBGM = m_pGameInstance->Get_Single_Sound("Fortress_Courtyard");
	m_pBGM->Set_Volume(0.5f);
	m_pBGM->Play();

	return S_OK;
}

void CLevel_Courtyard::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		Change_Level(ENUM_CLASS(LEVEL::ARENA));
	}

	else if (KEY_DOWN(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	Check_Collision();

	if (m_iNextLevel)
	{
		m_pGameInstance->Clear_Colliders();
		m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::ARENA));
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
	_float3 vInitPosition = { -0.f, 0.f, -100.f };

	// 플레이어가 있는지 체크하고 있으면 위치만 변경해줌.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))
			->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vInitPosition), 1.f));
		static_cast<CPlayer*>(pPlayer)->Set_Level(CurLevel);
		return S_OK;
	}

	//없으면 새로 생성해서 넣어줌
	CPlayer::DESC tDesc{};
	tDesc.eLevelID = CurLevel;
	tDesc.fSpeedPerSec = 5.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Player");
	tDesc.iNumPartObjects = CPlayer::PART_END;

	tDesc.WorldMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(vInitPosition.x, vInitPosition.y, vInitPosition.z);

	// 최초 게임 입장할때 어디에서 입장하던 스태틱에 생성해준다.
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Camera(const _wstring& strLayerTag)
{
#pragma region 프리카메라
	//CCamera_Free::DESC FreeCameraDesc = {};

	//FreeCameraDesc.eLevelID = CurLevel;
	//FreeCameraDesc.fSensor = 0.1f;

	//FreeCameraDesc.vEye = _float3(0.f, 20.f, -15.f);
	//FreeCameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	//FreeCameraDesc.fFov = XMConvertToRadians(60.f);
	//FreeCameraDesc.fNear = 0.1f;
	//FreeCameraDesc.fFar = 3000.f;
	//FreeCameraDesc.fSpeedPerSec = 30.f;
	//FreeCameraDesc.fRotationPerSec = XMConvertToRadians(180.f);
	//FreeCameraDesc.strName = TEXT("Camera_Free");

	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + FreeCameraDesc.strName,
	//	ENUM_CLASS(FreeCameraDesc.eLevelID), strLayerTag, &FreeCameraDesc)))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Camera(ENUM_CLASS(CurLevel), strLayerTag, FreeCameraDesc.strName)))
	//	return E_FAIL;
#pragma endregion 

	CGameObject* pPlayer = GET_PLAYER;

	_float3 vPos = {};
	XMStoreFloat3(&vPos, static_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION));
	
	CCamera_TPS::DESC tDesc = {};

	tDesc.eLevelID = CurLevel;
	tDesc.fSensor = 1.5f;
	tDesc.vOffset = _float3(-11.f, 15.f, -11.f);
	tDesc.fDeadZoneX = 2.5f;
	tDesc.fDeadZoneZ = 2.5f;
	tDesc.pTarget = pPlayer;

	tDesc.vEye = _float3(vPos.x + tDesc.vOffset.x, vPos.y + tDesc.vOffset.y, vPos.z + tDesc.vOffset.z);
	tDesc.vAt = _float3(vPos.x, vPos.y, vPos.z);
	tDesc.fFov = XMConvertToRadians(60.f);
	tDesc.fNear = 0.1f;
	tDesc.fFar = 1000.f;

	tDesc.fSpeedPerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Camera_TPS");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Camera(ENUM_CLASS(CurLevel), strLayerTag, tDesc.strName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
#pragma region 지형
	//CTerrain::DESC tDesc = {};
	//tDesc.eLevelID = CurLevel;
	//tDesc.fSpeedPerSec = 0.f;
	//tDesc.fRotationPerSec = 0.f;
	//tDesc.strName = TEXT("Terrain");

	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
	//	ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
	//	return E_FAIL;
#pragma endregion

	CSky::DESC tSkyDesc = {};
	tSkyDesc.eLevelID = CurLevel;
	tSkyDesc.fSpeedPerSec = 0.f;
	tSkyDesc.fRotationPerSec = 0.f;
	tSkyDesc.strName = TEXT("Sky");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tSkyDesc.strName,
		ENUM_CLASS(tSkyDesc.eLevelID), strLayerTag, &tSkyDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_UI(const _wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Courtyard::Load_Map(const _wstring& strMapFileTag)
{
	ifstream LoadFile(TEXT("../bin/DataFiles/MapData/") + strMapFileTag, ios::binary);

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
		tDesc.eLevelID = CurLevel;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
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
		tDesc.eLevelID = CurLevel;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.iNumPartObjects), sizeof(_uint));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
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
		tDesc.eLevelID = CurLevel;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.iNumPartObjects), sizeof(_uint));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
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
		tDesc.eLevelID = CurLevel;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.iNumPartObjects), sizeof(_uint));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.eType), sizeof(ITEM_TYPE));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Chest"), &tDesc)))
			return E_FAIL;
	}

	_uint iNumMonsters{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumMonsters), sizeof(_uint));

	m_MonsterDescs.reserve(iNumMonsters);

	for (_uint i = 0; i < iNumMonsters; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CMonster::DESC tDesc{};
		tDesc.eLevelID = CurLevel;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.iNumPartObjects), sizeof(_uint));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		m_MonsterDescs.push_back(tDesc);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + tDesc.strName,
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

HRESULT CLevel_Courtyard::Respawn_Objects()
{
	m_pGameInstance->Clear_ColliderGroup(ENUM_CLASS(COLLIDER_GROUP::MONSTER));
	m_pGameInstance->Clear_ColliderGroup(ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK));

	m_pGameInstance->Layer_Clear(ENUM_CLASS(CurLevel), TEXT("Layer_Monster"));

	for (auto& pMobDesc : m_MonsterDescs)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(CurLevel), TEXT("Prototype_GameObject_") + pMobDesc.strName,
			ENUM_CLASS(pMobDesc.eLevelID), TEXT("Layer_Monster"), &pMobDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CLevel_Courtyard::Check_Collision()
{
#ifdef _DEBUG
	m_pGameInstance->Reset_Colliders();
#endif // _DEBUG

	m_pGameInstance->Reset_SlidingVectors();

	/* 플레이어 */
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::PAWN), ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT));
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::PAWN), ENUM_CLASS(COLLIDER_GROUP::MONSTER));
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::PAWN), ENUM_CLASS(COLLIDER_GROUP::ITEM));

	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::WEAPON), ENUM_CLASS(COLLIDER_GROUP::MONSTER));
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::WEAPON), ENUM_CLASS(COLLIDER_GROUP::ENVIRONMENT));

	/* 몬스터 */
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::MONSTER), ENUM_CLASS(COLLIDER_GROUP::MONSTER));
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK), ENUM_CLASS(COLLIDER_GROUP::PAWN));
	m_pGameInstance->Intersect(ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET), ENUM_CLASS(COLLIDER_GROUP::PAWN));


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

	m_pBGM->Stop();
	Safe_Release(m_pBGM);

	m_MonsterDescs.clear();
}
