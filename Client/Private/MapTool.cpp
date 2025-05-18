#include "MapTool.h"
#include "GameInstance.h"

#include "Courtyard.h"
#include "Arena.h"
#include "Camera_Free.h"
#include "Bush.h"


CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{pDevice, pContext}
{
}

HRESULT CMapTool::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	/* �̰� ��¥ �³���? */
	vector<_wstring> EnvironmentFilters = {
		L"Grass", L"Bush", L"CheckPoint"
	};

	for (const auto& Pair : *m_pGameInstance->Get_Prototypes(ENUM_CLASS(LEVEL::TOOLS)))
	{
		for (const auto& KeyWord : EnvironmentFilters)
		{
			if (Pair.first.find(TEXT("Prototype_GameObject_")) != _wstring::npos &&
				Pair.first.find(KeyWord) != _wstring::npos)
				m_EnvironmentNames.push_back(m_pGameInstance->WStringToString(Pair.first));
		}
	}

	return S_OK;
}

void CMapTool::Update(_float fTimeDelta)
{
	if (nullptr == m_pMap)
		return;

	if (MOUSE_DOWN(DIMK::LBUTTON))
	{
		_float3 vInitPos = m_pMap->Get_PickedPos_World();

		CEnvironment_Object::DESC tDesc = {};
		tDesc.eLevelID = LEVEL::TOOLS;
		tDesc.fRotationPerSec = 0.f;
		tDesc.fSpeedPerSec = 0.f;
		tDesc.strName = m_strName;
		tDesc.WorldMatrix = XMMatrixTranslation(vInitPos.x, vInitPos.y, vInitPos.z);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_MapTool_Object"), &tDesc)))
			return;
	}
}

HRESULT CMapTool::Render()
{
	MapTool();

	return S_OK;
}

HRESULT CMapTool::MapTool()
{
	ImGui::Begin(u8"�� ��");

	Map_Menu();
	Load_Map_Menu();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	if (ImGui::BeginTabBar(u8"�� ����"))
	{
		if (ImGui::BeginTabItem(u8"ȯ��"))
		{
			Environment_ListBox();
			if (ImGui::Button(u8"����"))
			{

			}
			ImGui::SameLine();
			if(ImGui::Button(u8"����"))
			{

			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"��"))
		{
			Monster_ListBox();

			ImGui::EndTabItem();
		}
	}
	ImGui::SameLine();
	ImGui::EndTabBar();

	ImGui::End();

	return S_OK;
}

void CMapTool::Map_Menu()
{
	static MAP eCurrntMap = { MAP_END };
	static MAP ePreMap = { MAP_END };
	const _char* szMaps[] = { "CourtYard", "Main", "Arena" };

	if (ImGui::BeginCombo(u8"�� ����", eCurrntMap == MAP_END ? u8"����" : szMaps[eCurrntMap]))
	{
		for (_uint i = 0; i < IM_ARRAYSIZE(szMaps); i++)
		{
			_bool bSelected = { eCurrntMap == i };
			if (ImGui::Selectable(szMaps[i], bSelected))
				eCurrntMap = static_cast<MAP>(i);

			if (bSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ePreMap != eCurrntMap && eCurrntMap != MAP_END)
	{
		if (FAILED(Craete_Map(eCurrntMap, TEXT("Layer_Map"))))
		{
			MSG_BOX("�� ���� ����,,");
			return;
		}
			
		ePreMap = eCurrntMap;
	}
}

void CMapTool::File_Menu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"����"))
		{
			if (ImGui::MenuItem(u8"����"))
			{
				if (m_strMapFileTag.empty())
					MSG_BOX("���� �����ϰ� �����ؾ���,,,");
				else
					Save_Map("../bin/Resources/MapData/" + m_strMapFileTag);
			}
			if (ImGui::MenuItem(u8"�ҷ�����"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../bin/Resources/MapData/";
				config.fileName = "ChooseFile";

				ImGuiFileDialog::Instance()->OpenDialog(
					"LoadFile",            // ���̾�α� Key
					u8"���� ����",              // Ÿ��Ʋ
					".Map",        // ���� (���� �� ����),
					config                    // ���� ���
				);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

HRESULT CMapTool::Craete_Map(MAP iMapIdx, const _wstring& strLayerTag)
{
	_wstring strName;

	switch (iMapIdx)
	{
	case MAP::COURTYARD:
		m_strMapFileTag = "Courtyard.Map";
		strName = TEXT("Courtyard");
		break;

	case MAP::MAIN:
		m_strMapFileTag = "Main.Map";
		strName = TEXT("Main");
		break;

	case MAP::ARENA:
		m_strMapFileTag = "Arena.Map";
		strName = TEXT("Arena");
		break;
	default:
		break;
	}

	CMap* pMap = static_cast<CMap*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), strLayerTag));
	if (nullptr != pMap)
	{
		Safe_Release(m_pMap);
		pMap->Set_Dead(true);
	}
		
	
	CMap::DESC tDesc = {};
	tDesc.eLevelID = LEVEL::TOOLS;
	tDesc.fRotationPerSec = 0.f;
	tDesc.fSpeedPerSec = 0.f;
	tDesc.strName = strName;

	if(FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	m_pMap = static_cast<CMap*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), strLayerTag));
	if (nullptr != m_pMap)
		Safe_AddRef(m_pMap);

	return S_OK;
}

HRESULT CMapTool::Environment_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// ����Ʈ�ڽ� ���
	if (ImGui::BeginListBox(u8"������Ÿ��", ImVec2(300, 100)))
	{
		for (_uint i = 0; i < m_EnvironmentNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_EnvironmentNames[i].c_str(), bSelected))
				iCurrentObjIndex = i;
		}
		ImGui::EndListBox();
	}

	switch (iCurrentObjIndex)
	{
	case BUSH:
		m_strName = TEXT("Bush");
		break;

	case CHECKPOINT:
		m_strName = TEXT("CheckPoint");
		break;

	case GRASS:
		m_strName = TEXT("Grass");
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CMapTool::Chest_ListBox()
{
	return S_OK;
}

HRESULT CMapTool::Monster_ListBox()
{
	return S_OK;
}

void CMapTool::Load_Map_Menu()
{
	if (ImGuiFileDialog::Instance()->Display("LoadFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK �����ٸ�
		{
			_string FilePath = ImGuiFileDialog::Instance()->GetFilePathName();
			// ���ϴ� �۾� ����
			Load_Map(FilePath);
		}

		ImGuiFileDialog::Instance()->Close(); // �� �ݾ���� �ٽ� ����
	}
}

HRESULT CMapTool::Save_Map(const _string& strMapPath)
{
	ofstream OutFile(strMapPath, ios::binary);

	if (!OutFile.is_open())
	{
		MSG_BOX("���� ���� ����,,");
		return E_FAIL;
	}

	list<CGameObject*>* pMapList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Map"));
	if (nullptr == pMapList)
	{
		/* SR�� �̰� �ϳ� �߰� ���ؼ� �����,, */
		OutFile.write(reinterpret_cast<const _char*>(0), sizeof(_uint));
	}
	else
	{
		/* ������Ʈ ���� ���� �ٵ� ��ó�� ���ε� ���� �Ѱ� �ƴѰ�? �ϴ� �� */
		_uint iNumMaps = static_cast<_uint>(pMapList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumMaps), sizeof(_uint));

		for (auto& pMap : *pMapList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pMap->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* ���ڿ� ����� */
			_int iSaveLength = {};
			_float4x4 WorldMatrix  = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec    = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strPrototype  =  pMap->Get_Name();
			iSaveLength = static_cast<_int>(strPrototype.length());

			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix),		sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec),	sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength),		sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strPrototype.c_str()), sizeof(_tchar) * iSaveLength);
		}
	}

	OutFile.close();

	MSG_BOX("���� ���� ����,,!");

	return S_OK;
}

HRESULT CMapTool::Load_Map(const _string& strMapPath)
{
	m_pGameInstance->Object_Clear(ENUM_CLASS(LEVEL::TOOLS));

	if (FAILED(Craete_Camera(TEXT("Layer_Camera"))))
	{
		MSG_BOX("���� ī�޶� ��¼��,,,");
		return E_FAIL;
	}

	ifstream LoadFile(strMapPath, ios::binary);

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
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_MapTool"), &tDesc)))
			return E_FAIL;
	}

	LoadFile.close();
	MSG_BOX("���� �ε� ����");

	return S_OK;
}

HRESULT CMapTool::Craete_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::TOOLS;
	tDesc.fSensor = 0.1f;

	tDesc.vEye = _float3(0.f, 20.f, -15.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);
	tDesc.fFov = XMConvertToRadians(60.f);
	tDesc.fNear = 0.1f;
	tDesc.fFar = 3000.f;
	tDesc.fSpeedPerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.strName = TEXT("Camera_Free");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

CMapTool* CMapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapTool* pInstance = new CMapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapTool::Free()
{
	__super::Free();

	Safe_Release(m_pMap);
}
