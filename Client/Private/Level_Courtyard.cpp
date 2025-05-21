#include "Level_Courtyard.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "Terrain.h"
#include "Camera_Free.h"

#include "Monster.h"
#include "Player.h"

#include "Courtyard.h"

#include "Environment_Object.h"

CLevel_Courtyard::CLevel_Courtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Courtyard::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Pawn(TEXT("Layer_Pawn"))))
		return E_FAIL;

	if (FAILED(Load_Map(TEXT("Courtyard.Map"))))
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
	SetWindowText(g_hWnd, TEXT("�����÷��� �����Դϴ�."));

	return S_OK;
}

HRESULT CLevel_Courtyard::Ready_Layer_Pawn(const _wstring& strLayerTag)
{
	//�� ������ �÷��̾� ������ġ
	_float3 vInitPosition = { 10.f, 5.f, 10.f };

	// �÷��̾ �ִ��� üũ�ϰ� ������ ��ġ�� ��������.
	auto pPlayer = GET_PLAYER;
	if (pPlayer)
	{
		static_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))
			->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&vInitPosition), 1.f));
		static_cast<CPawn*>(pPlayer)->Set_Level(LEVEL::COURTYARD);
		return S_OK;
	}

	//������ ���� �����ؼ� �־���
	CPlayer::DESC tDesc{};
	tDesc.eLevelID = LEVEL::STATIC;
	tDesc.fSpeedPerSec = 20.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Player");

	
	tDesc.WorldMatrix = XMMatrixTranslation(vInitPosition.x, vInitPosition.y, vInitPosition.z);

	// ���� ���� �����Ҷ� ��𿡼� �����ϴ� ����ƽ�� �������ش�.
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

HRESULT CLevel_Courtyard::Load_Map(const _wstring& strMapFileTag)
{
	m_pGameInstance->Object_Clear(ENUM_CLASS(LEVEL::TOOLS));

	ifstream LoadFile(TEXT("../bin/Resources/MapData/") + strMapFileTag, ios::binary);

	if (!LoadFile.is_open())
	{
		MSG_BOX("���� ���� ����,,");
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
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_EnvironmentObject"), &tDesc)))
			return E_FAIL;
	}

	LoadFile.close();
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
