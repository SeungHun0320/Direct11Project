#include "MapTool.h"
#include "GameInstance.h"

#include "Courtyard.h"
#include "Arena.h"
#include "Camera_Free.h"
#include "Bush.h"
#include "Chest.h"
#include "Item.h"
#include "Blob.h"

#define MAX_SCALE 100.f
#define MIN_ANGLE -180.f
#define MAX_ANGLE 180.f

CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{pDevice, pContext}
{
}

HRESULT CMapTool::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Add_ListBoxName();

	/* 처음 켰을때 수정모드 켜져 있도록 */
	m_bMode[MODIFY] = true;

	return S_OK;
}

void CMapTool::Add_ListBoxName()
{	
	/* 이게 진짜 맞나요? */
	vector<_wstring> EnvironmentFilters = {
		L"Grass", L"Bush", L"CheckPoint"
	};

	vector<_wstring> ItemFilters = {
		L"Potion"
	};

	vector<_wstring> MonsterFilters = {
		L"Blob"
	};

	for (const auto& Pair : *m_pGameInstance->Get_Prototypes(ENUM_CLASS(LEVEL::TOOLS)))
	{
		if (Pair.first.find(TEXT("Prototype_GameObject_")) == std::wstring::npos)
			continue;

		for (const auto& KeyWord : EnvironmentFilters)
		{
			if (Pair.first.find(KeyWord) != _wstring::npos)
				m_ProtoEnvironmentNames.push_back(m_pGameInstance->WStringToString(Pair.first));
		}

		for (const auto& KeyWord : ItemFilters)
		{
			if (Pair.first.find(KeyWord) != _wstring::npos)
				m_ProtoItemNames.push_back(m_pGameInstance->WStringToString(Pair.first));
		}

		for (const auto& KeyWord : MonsterFilters)
		{
			if (Pair.first.find(KeyWord) != _wstring::npos)
				m_ProtoMonsterNames.push_back(m_pGameInstance->WStringToString(Pair.first));
		}
	}
}

void CMapTool::Update(_float fTimeDelta)
{
	Key_Input();
	if (nullptr == m_pMap || m_bMode[NORMAL])
		return;

	if (m_bMode[MODIFY])
	{
		if (MOUSE_DOWN(DIMK::LBUTTON))
		{
			CGameObject* pGameObject = m_pGameInstance->Find_Picked_Object(ENUM_CLASS(LEVEL::TOOLS), m_strLayerTag);
			if (nullptr == pGameObject)
				return;

			if (nullptr != m_pModifyObject)
				Safe_Release(m_pModifyObject);

			m_pModifyObject = pGameObject;
			Safe_AddRef(m_pModifyObject);
			m_bFirst = true;
		}

		if (MOUSE_DOWN(DIMK::RBUTTON))
		{
			CGameObject* pGameObject = m_pGameInstance->Find_Picked_Object(ENUM_CLASS(LEVEL::TOOLS), m_strLayerTag);
			if (nullptr == pGameObject)
				return;
			
			pGameObject->Set_Dead(true);
		}

	}

	if (m_bMode[CREATE] && MOUSE_DOWN(DIMK::LBUTTON))
	{
		_float3 vInitPos = m_pMap->Get_PickedPos_World();
		XMStoreFloat3(&vInitPos, XMVectorRound(XMLoadFloat3(&vInitPos)));

		if (m_bLists[ENVIRONMENT])
		{
			CEnvironment_Object::DESC tDesc = {};
			tDesc.eLevelID = LEVEL::TOOLS;
			tDesc.fRotationPerSec = 0.f;
			tDesc.fSpeedPerSec = 0.f;
			tDesc.strName = m_strName;
			tDesc.WorldMatrix = XMMatrixTranslation(vInitPos.x, vInitPos.y, vInitPos.z);

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
				ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Environment_Object"), &tDesc)))
				return;

			Add_Modify_ListBox(m_EnvironmentNames, tDesc.strName);
		}
		else if (m_bLists[ITEM])
		{
			// 여기서 아이템 Desc 만들어서 생성
			CItem::DESC tDesc{};
			tDesc.eLevelID = LEVEL::TOOLS;
			tDesc.fRotationPerSec = m_fRotationPerSec;
			tDesc.fSpeedPerSec = m_fSpeedPerSec;
			tDesc.strName = m_strName;
			tDesc.WorldMatrix = XMMatrixTranslation(vInitPos.x, vInitPos.y, vInitPos.z);

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
				ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Item"), &tDesc)))
				return;
		}
		else if (m_bLists[CHEST])
		{
			// 여기서 상자 Desc 만들어서 생성 (상자 자체는 환경오브젝트로 가되, 애니메이션도 있고 뭐 이것저것 있을 예정)
			CChest::DESC tDesc = {};
			tDesc.eLevelID = LEVEL::TOOLS;
			tDesc.fRotationPerSec = m_fRotationPerSec;
			tDesc.fSpeedPerSec = m_fSpeedPerSec;
			tDesc.strName = m_strName;
			tDesc.WorldMatrix = XMMatrixTranslation(vInitPos.x, vInitPos.y, vInitPos.z);

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
				ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Chest"), &tDesc)))
				return;

			Add_Modify_ListBox(m_ChestNames, tDesc.strName);
		}
		else if (m_bLists[ENEMY])
		{
			// 여기서 적 Desc 만들어서 생성
			CMonster::DESC tDesc = {};
			tDesc.eLevelID = LEVEL::TOOLS;
			tDesc.fRotationPerSec = m_fRotationPerSec;
			tDesc.fSpeedPerSec = m_fSpeedPerSec;
			tDesc.strName = m_strName;
			tDesc.WorldMatrix = XMMatrixTranslation(vInitPos.x, vInitPos.y, vInitPos.z);

			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
				ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Monster"), &tDesc)))
				return;

			Add_Modify_ListBox(m_MonsterNames, tDesc.strName);
		}
	}
}

HRESULT CMapTool::Render()
{
	MapTool();
	AnimMesh_Menu();

	return S_OK;
}

void CMapTool::Key_Input()
{
	if (KEY_DOWN(DIK_V))
	{
		m_bMode[CREATE] = !m_bMode[CREATE];
		m_bMode[MODIFY] = !m_bMode[MODIFY];
	}
	if (KEY_DOWN(DIK_B))
		m_bMode[NORMAL] = !m_bMode[NORMAL];

	if (KEY_DOWN(DIK_Z))
	{
		if (ImGuizmo::TRANSLATE == m_eGizmoOp)
			m_eGizmoOp = ImGuizmo::ROTATE;
		else if (ImGuizmo::ROTATE == m_eGizmoOp)
			m_eGizmoOp = ImGuizmo::TRANSLATE;
	}
}

HRESULT CMapTool::MapTool()
{
	ImGui::Begin(u8"맵 툴");

	Map_Menu();
	Load_Map_Menu();
	Change_Mode();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	if (ImGui::BeginTabBar(u8"모델 선택"))
	{
		if (ImGui::BeginTabItem(u8"환경"))
		{
			Seleted_List(ENVIRONMENT);
			m_strLayerTag = TEXT("Layer_Environment_Object");
			Environment_ListBox();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"아이템"))
		{
			Seleted_List(ITEM);
			m_strLayerTag = TEXT("Layer_Item");
			Item_ListBox();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"상자"))
		{
			Seleted_List(CHEST);
			m_strLayerTag = TEXT("Layer_Chest");
			Chest_ListBox();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"적"))
		{
			Seleted_List(ENEMY);
			m_strLayerTag = TEXT("Layer_Monster");
			Monster_ListBox();

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	On_Modify_Object();


	ImGui::End();

	return S_OK;
}

void CMapTool::Map_Menu()
{
	const _char* szMaps[] = { "CourtYard", "Main", "Arena", "Shop"};

	if (ImGui::BeginCombo(u8"맵 선택", m_eCurrentMap == MAP_END ? u8"선택" : szMaps[m_eCurrentMap]))
	{
		for (_uint i = 0; i < IM_ARRAYSIZE(szMaps); i++)
		{
			_bool bSelected = { m_eCurrentMap == i };
			if (ImGui::Selectable(szMaps[i], bSelected))
				m_eCurrentMap = static_cast<MAP>(i);

			if (bSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (m_ePreMap != m_eCurrentMap && m_eCurrentMap != MAP_END)
	{
		if (FAILED(Craete_Map(m_eCurrentMap, TEXT("Layer_Map"))))
		{
			MSG_BOX("맵 생성 실패,,");
			return;
		}
			
		m_ePreMap = m_eCurrentMap;
	}
}

void CMapTool::File_Menu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"파일"))
		{
			if (ImGui::MenuItem(u8"저장"))
			{
				if (m_strMapFileTag.empty())
					MSG_BOX("맵을 선택하고 저장해야지,,,");
				else
					Save_Map("../bin/Resources/MapData/" + m_strMapFileTag);
			}
			if (ImGui::MenuItem(u8"불러오기"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../bin/Resources/MapData/";
				config.fileName = "ChooseFile";

				ImGuiFileDialog::Instance()->OpenDialog(
					"LoadFile",            // 다이얼로그 Key
					u8"파일 선택",              // 타이틀
					".Map",        // 필터 (여러 개 가능),
					config                    // 시작 경로
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

	case MAP::SHOP:
		m_strMapFileTag = "Shop.Map";
		strName = TEXT("Shop");
		break;
	default:
		break;
	}

	CMap* pMap = static_cast<CMap*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), strLayerTag));
	if (nullptr != pMap)
	{
		Safe_Release(m_pMap);
		m_pGameInstance->Object_Clear(ENUM_CLASS(LEVEL::TOOLS));
		if (FAILED(Craete_Camera(TEXT("Layer_Camera"))))
		{
			MSG_BOX("저는 카메라도 어쩌구,,,");
			return E_FAIL;
		}
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

HRESULT CMapTool::On_Modify_Object()
{
	if (nullptr == m_pModifyObject)
		return E_FAIL;

	CTransform* pTransform = dynamic_cast<CTransform*>(m_pModifyObject->Get_Component(TEXT("Com_Transform")));
	if (nullptr == pTransform)
		return E_FAIL;

	ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

	_float4x4  ViewMatrix{}, ProjMatrix{};

	ViewMatrix = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW);
	ProjMatrix = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ);

	ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

	_float fSnapValue[3] = { 0.25f, 0.25f, 0.25f };

	if (ImGuizmo::Manipulate(
		reinterpret_cast<_float*>(&ViewMatrix),
		reinterpret_cast<_float*>(&ProjMatrix),
		m_eGizmoOp,
		ImGuizmo::LOCAL,
		reinterpret_cast<_float*>(pTransform->Get_WorldMatrix_Float4x4()),
		nullptr,
		fSnapValue))
	{

	}

	Adj_Scale_Angle();

	_vector vPosition{}, vScale{}, vQuat{}, vTmp{};

	XMMatrixDecompose(&vScale, &vTmp, &vPosition, pTransform->Get_WorldMatrix());

	if (m_bFirst)
	{
		m_fRotationPerSec = pTransform->Get_RotationPerSec();
		m_fSpeedPerSec = pTransform->Get_SpeedPerSec();
		XMStoreFloat3(&m_vScale, vScale);
		XMStoreFloat3(&m_vInitPos, vPosition);
		m_bFirst = false;
	}

	if (ImGui::Button(u8"이동속도, 회전속도 적용"))
	{
		pTransform->Set_SpeedPerSec(m_fSpeedPerSec);
		pTransform->Set_RotationPerSec(m_fRotationPerSec);
	}

	vQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x), XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	pTransform->Set_Matrix(XMMatrixAffineTransformation(XMLoadFloat3(&m_vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), vQuat, vPosition));

	return S_OK;
}

void CMapTool::Adj_Scale_Angle()
{
	if (ImGui::RadioButton(u8"전체 스케일 변경", !m_bAllScale))
	{
		m_bAllScale = !m_bAllScale;
	}
	ImGui::PushItemWidth(130.f);

	ImGui::Separator();
	ImGui::SliderFloat(u8"이동속도", &m_fSpeedPerSec, 0, MAX_SCALE);
	ImGui::SameLine();
	ImGui::InputFloat("##SpeedPerSec", &m_fSpeedPerSec);
	ImGui::SliderFloat(u8"회전속도", &m_fRotationPerSec, 0, MAX_SCALE);
	ImGui::SameLine();
	ImGui::InputFloat("##RotationPerSec", &m_fRotationPerSec);
	ImGui::Separator();

	if (m_bAllScale)
	{
		ImGui::SliderFloat(u8"스케일 X", &m_vScale.x, 1.f, MAX_SCALE);
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleX", &m_vScale.x);
		ImGui::SliderFloat(u8"스케일 Y", &m_vScale.y, 1.f, MAX_SCALE);
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleY", &m_vScale.y);
		ImGui::SliderFloat(u8"스케일 Z", &m_vScale.z, 1.f, MAX_SCALE);
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleZ", &m_vScale.z);
	}
	else
	{
		ImGui::SliderFloat(u8"스케일", &m_vScale.x, 1.f, MAX_SCALE);
		ImGui::SameLine();
		ImGui::InputFloat("##Scale", &m_vScale.x);
		m_vScale.z = m_vScale.y = m_vScale.x;
	}

	/* 회전 먼가 이상함, y가 x축 회전되고 x가 y축 회전 됨 그래서 일단 바꿔놨음 */
	/* 해결, 쿼터니움 구하는 함수가 y x z 를 인자로 요구함*/
	ImGui::Separator();
	ImGui::SliderFloat(u8"회전 X", &m_vAngle.x, MIN_ANGLE, MAX_ANGLE);
	ImGui::SameLine();
	ImGui::InputFloat("##AngleX", &m_vAngle.x);
	ImGui::SliderFloat(u8"회전 Y", &m_vAngle.y, MIN_ANGLE, MAX_ANGLE);
	ImGui::SameLine();
	ImGui::InputFloat("##AngleY", &m_vAngle.y);
	ImGui::SliderFloat(u8"회전 Z", &m_vAngle.z, MIN_ANGLE, MAX_ANGLE);
	ImGui::SameLine();
	ImGui::InputFloat("##AngleZ", &m_vAngle.z);

}

void CMapTool::Seleted_List(LIST eList)
{
	for (_uint i = 0; i < LIST_END; i++)
		m_bLists[i] = false;

	m_bLists[eList] = true;
}

HRESULT CMapTool::Environment_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"환경 프로토타입");
	if (ImGui::BeginListBox("##ProtoEnv", ImVec2(300, 100)))
	{
		for (_uint i = 0; i < m_ProtoEnvironmentNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_ProtoEnvironmentNames[i].c_str(), bSelected))
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

HRESULT CMapTool::Item_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"아이템 프로토타입");
	if (ImGui::BeginListBox("##ProtoItem", ImVec2(300, 100)))
	{
		for (_uint i = 0; i < m_ProtoItemNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_ProtoItemNames[i].c_str(), bSelected))
				iCurrentObjIndex = i;
		}
		ImGui::EndListBox();
	}

	/* 바꾸삼 */
	switch (iCurrentObjIndex)
	{
	case POTION:
		m_strName = TEXT("Potion");
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CMapTool::Chest_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };
	const _char* szChests[] =
	{ u8"돈 상자", u8"아이템 상자"};

	ImGui::Text(u8"상자 종류");
	ImGui::ListBox(u8"##ChestTypes", &iCurrentObjIndex, szChests, IM_ARRAYSIZE(szChests));

	/* 상자는 DESC를 멤버변수로 들고 있어야 할 것 같음 */
	switch (iCurrentObjIndex)
	{
	case CHEST_TYPE::CT_MONEY:
		break;

	case CHEST_TYPE::CT_ITEM:
		break;

	default:
		break;
	}

	m_strName = TEXT("Chest");

	return S_OK;
}

HRESULT CMapTool::Monster_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"몹 프로토타입");
	if (ImGui::BeginListBox("##ProtoMob", ImVec2(300, 100)))
	{
		for (_uint i = 0; i < m_ProtoMonsterNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_ProtoMonsterNames[i].c_str(), bSelected))
				iCurrentObjIndex = i;
		}
		ImGui::EndListBox();
	}

	/* 바꾸삼 바꿨음 */
	switch (iCurrentObjIndex)
	{
	case BLOB:
		m_strName = TEXT("Blob");
		break;

	default:
		break;
	}

	return S_OK;
}

void CMapTool::AnimMesh_Menu()
{
	if (ImGui::Begin(u8"하이~ 어 라키"))
	{
		if (ImGui::BeginTabBar(u8"애님메쉬 수정 창"))
		{
			if (ImGui::BeginTabItem(u8"환경"))
			{
				AnimMesh_Environment_ListBox();

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(u8"상자"))
			{
				AnimMesh_Chest_ListBox();

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(u8"적"))
			{
				AnimMesh_Monster_ListBox();

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void CMapTool::Add_Modify_ListBox(vector<_string>& vecNames, const _wstring& strName)
{
	_string strNumObjects = to_string(vecNames.size());
	strNumObjects += "_";
	vecNames.push_back(strNumObjects + m_pGameInstance->WStringToString(strName));
}

HRESULT CMapTool::AnimMesh_Chest_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"상자~쓰");
	if (ImGui::BeginListBox("##ChestListBox", ImVec2(150, 500)))
	{
		for (_uint i = 0; i < m_ChestNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_ChestNames[i].c_str(), bSelected))
			{
				iCurrentObjIndex = i;

				CGameObject* pGameObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Chest"), i);
				if (nullptr == pGameObject)
					return E_FAIL;

				if (nullptr != m_pModifyObject)
					Safe_Release(m_pModifyObject);

				m_pModifyObject = pGameObject;
				Safe_AddRef(m_pModifyObject);
				m_bFirst = true;
			}
				
		}
		ImGui::EndListBox();
	}

	return S_OK;

}

HRESULT CMapTool::AnimMesh_Monster_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"몬스터~쓰");
	if (ImGui::BeginListBox("##MobListBox", ImVec2(150, 500)))
	{
		for (_uint i = 0; i < m_MonsterNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_MonsterNames[i].c_str(), bSelected))
			{
				iCurrentObjIndex = i;

				CGameObject* pGameObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Monster"), i);
				if (nullptr == pGameObject)
					return E_FAIL;

				if (nullptr != m_pModifyObject)
					Safe_Release(m_pModifyObject);

				m_pModifyObject = pGameObject;
				Safe_AddRef(m_pModifyObject);
				m_bFirst = true;
			}
			
		}
		ImGui::EndListBox();
	}

	return S_OK;
}

HRESULT CMapTool::AnimMesh_Environment_ListBox()
{
	static _int iCurrentObjIndex = { -1 }, iOldObjType = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"환경오브젝~츠");
	if (ImGui::BeginListBox("##EnvListBox", ImVec2(150, 500)))
	{
		for (_uint i = 0; i < m_EnvironmentNames.size(); ++i)
		{
			_bool bSelected = (iCurrentObjIndex == i);
			if (ImGui::Selectable(m_EnvironmentNames[i].c_str(), bSelected))
			{
				iCurrentObjIndex = i;

				CGameObject* pGameObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Environment_Object"), i);
				if (nullptr == pGameObject)
					return E_FAIL;

				if (nullptr != m_pModifyObject)
					Safe_Release(m_pModifyObject);

				m_pModifyObject = pGameObject;
				Safe_AddRef(m_pModifyObject);
				m_bFirst = true;
			}
		}
		ImGui::EndListBox();
	}

	/* 바꾸삼 바꿨음 */
	switch (iCurrentObjIndex)
	{
	case BLOB:
		m_strName = TEXT("Blob");
		break;

	default:
		break;
	}

	return S_OK;
}

void CMapTool::Change_Mode()
{
	if (ImGui::RadioButton(u8"생성모드", m_bMode[CREATE]))
	{
		m_bMode[CREATE] = !m_bMode[CREATE];
		m_bMode[MODIFY] = false;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"수정모드", m_bMode[MODIFY]))
	{
		m_bMode[MODIFY] = !m_bMode[MODIFY];
		m_bMode[CREATE] = false;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"잠금", m_bMode[NORMAL]))
	{
		m_bMode[NORMAL] = !m_bMode[NORMAL];
	}
	ImGui::Text(u8"키를 눌러 모드 전환");
	ImGui::Text(u8"V : 생성/수정 || B : 잠금 ");
}

void CMapTool::Load_Map_Menu()
{
	if (ImGuiFileDialog::Instance()->Display("LoadFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			_string FilePath = ImGuiFileDialog::Instance()->GetFilePathName();
			// 원하는 작업 수행
			Load_Map(FilePath);
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}
}

HRESULT CMapTool::Save_Map(const _string& strMapPath)
{
	ofstream OutFile(strMapPath, ios::binary);

	if (!OutFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	_int iZero = { 0 };

	/* 맵 저장 */
	list<CGameObject*>* pMapList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Map"));
	if (nullptr == pMapList)
	{
		/* SR때 이거 하나 추가 안해서 고생함,, */
		OutFile.write(reinterpret_cast<const _char*>(&iZero), sizeof(_uint));
	}
	else
	{
		/* 오브젝트 갯수 저장 근데 어처피 맵인데 맵은 한개 아닌가? 일단 고 */
		_uint iNumMaps = static_cast<_uint>(pMapList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumMaps), sizeof(_uint));

		for (auto& pMap : *pMapList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pMap->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* 문자열 저장용 */
			_int iSaveLength = {};
			_float4x4 WorldMatrix  = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec    = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strName	   =  pMap->Get_Name();
			iSaveLength = static_cast<_int>(strName.length());

			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength), sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strName.c_str()), sizeof(_tchar) * iSaveLength);
			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix),		sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec),	sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));

		}
	}
	/* 환경 오브젝트 저장 */
	list<CGameObject*>* pEnvironment_ObjectList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Environment_Object"));
	if (nullptr == pEnvironment_ObjectList)
	{
		/* SR때 이거 하나 추가 안해서 고생함,, */
		OutFile.write(reinterpret_cast<const _char*>(&iZero), sizeof(_uint));
	}
	else
	{
		/* 오브젝트 갯수 저장 */
		_uint iNumEnvironmnet_Objects = static_cast<_uint>(pEnvironment_ObjectList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumEnvironmnet_Objects), sizeof(_uint));

		for (auto& pObject : *pEnvironment_ObjectList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* 문자열 저장용 */
			_int iSaveLength = {};
			_float4x4 WorldMatrix = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strPrototype = pObject->Get_Name();
			iSaveLength = static_cast<_int>(strPrototype.length());

			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength), sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strPrototype.c_str()), sizeof(_tchar) * iSaveLength);
			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix), sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec), sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));

		}
	}
	/* 아이템 저장 */
	list<CGameObject*>* pItemList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Item"));
	if (nullptr == pItemList)
	{
		/* SR때 이거 하나 추가 안해서 고생함,, */
		OutFile.write(reinterpret_cast<const _char*>(&iZero), sizeof(_uint));
	}
	else
	{
		/* 아이템 갯수 저장 */
		_uint iNumItems = static_cast<_uint>(pItemList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumItems), sizeof(_uint));

		for (auto& pObject : *pItemList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* 문자열 저장용 */
			_int iSaveLength = {};
			_float4x4 WorldMatrix = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strPrototype = pObject->Get_Name();
			iSaveLength = static_cast<_int>(strPrototype.length());

			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength), sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strPrototype.c_str()), sizeof(_tchar) * iSaveLength);
			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix), sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec), sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));

		}
	}
	/* 상자 저장 */
	list<CGameObject*>* pChestList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Chest"));
	if (nullptr == pChestList)
	{
		/* SR때 이거 하나 추가 안해서 고생함,, */
		OutFile.write(reinterpret_cast<const _char*>(&iZero), sizeof(_uint));
	}
	else
	{
		/* 상자 갯수 저장 */
		_uint iNumChests = static_cast<_uint>(pChestList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumChests), sizeof(_uint));

		for (auto& pObject : *pChestList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* 문자열 저장용 */
			_int iSaveLength = {};
			_float4x4 WorldMatrix = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strPrototype = pObject->Get_Name();
			iSaveLength = static_cast<_int>(strPrototype.length());

			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength), sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strPrototype.c_str()), sizeof(_tchar) * iSaveLength);
			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix), sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec), sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));

		}
	}
	/* 몬스터 저장 */
	list<CGameObject*>* pMonsterList = m_pGameInstance->Find_ObjectList(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Monster"));
	if (nullptr == pMonsterList)
	{
		/* SR때 이거 하나 추가 안해서 고생함,, */
		OutFile.write(reinterpret_cast<const _char*>(&iZero), sizeof(_uint));
	}
	else
	{
		/* 몬스터 갯수 저장 */
		_uint iNumMonsters = static_cast<_uint>(pMonsterList->size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumMonsters), sizeof(_uint));

		for (auto& pObject : *pMonsterList)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(TEXT("Com_Transform")));
			if (nullptr == pTransform)
				return E_FAIL;

			/* 문자열 저장용 */
			_int iSaveLength = {};
			_float4x4 WorldMatrix = *pTransform->Get_WorldMatrix_Float4x4();
			_float fSpeedPerSec = pTransform->Get_SpeedPerSec();
			_float fRotationPerSec = pTransform->Get_RotationPerSec();
			_wstring strPrototype = pObject->Get_Name();
			iSaveLength = static_cast<_int>(strPrototype.length());

			OutFile.write(reinterpret_cast<const _char*>(&iSaveLength), sizeof(_int));
			OutFile.write(reinterpret_cast<const _char*>(strPrototype.c_str()), sizeof(_tchar) * iSaveLength);
			OutFile.write(reinterpret_cast<const _char*>(&WorldMatrix), sizeof(_float4x4));
			OutFile.write(reinterpret_cast<const _char*>(&fSpeedPerSec), sizeof(_float));
			OutFile.write(reinterpret_cast<const _char*>(&fRotationPerSec), sizeof(_float));

		}
	}

	OutFile.close();

	MSG_BOX("파일 저장 성공,,!");

	return S_OK;
}

HRESULT CMapTool::Load_Map(const _string& strMapPath)
{
	m_EnvironmentNames.clear();
	m_ChestNames.clear();
	m_MonsterNames.clear();

	m_pGameInstance->Object_Clear(ENUM_CLASS(LEVEL::TOOLS));

	if (FAILED(Craete_Camera(TEXT("Layer_Camera"))))
	{
		MSG_BOX("저는 카메라도 어쩌구,,,");
		return E_FAIL;
	}

	ifstream LoadFile(strMapPath, ios::binary);

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
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Map"), &tDesc)))
			return E_FAIL;

		if (tDesc.strName == L"Courtyard")
			m_eCurrentMap = COURTYARD;
		else if (tDesc.strName == L"Main")
			m_eCurrentMap = MAIN;
		else if (tDesc.strName == L"Arena")
			m_eCurrentMap = ARENA;
		else if (tDesc.strName == L"Shop")
			m_eCurrentMap = SHOP;
		
		m_ePreMap = m_eCurrentMap;
		
		m_pMap = static_cast<CMap*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), TEXT("Layer_Map")));
		if (nullptr != m_pMap)
			Safe_AddRef(m_pMap);
	}

	_uint iNumEnvironment_Objects{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumEnvironment_Objects), sizeof(_uint));

	for (_uint i = 0; i < iNumEnvironment_Objects; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CEnvironment_Object::DESC tDesc{};
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Environment_Object"), &tDesc)))
			return E_FAIL;

		Add_Modify_ListBox(m_EnvironmentNames, tDesc.strName);
	}

	_uint iNumItem{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumItem), sizeof(_uint));

	for (_uint i = 0; i < iNumItem; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CItem::DESC tDesc{};
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
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
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Chest"), &tDesc)))
			return E_FAIL;

		Add_Modify_ListBox(m_ChestNames, tDesc.strName);
	}

	_uint iNumMonsters{};
	LoadFile.read(reinterpret_cast<_char*>(&iNumMonsters), sizeof(_uint));

	for (_uint i = 0; i < iNumMonsters; i++)
	{
		_int iLoadLength{};
		_float4x4 WorldMatrix{};

		CMonster::DESC tDesc{};
		tDesc.eLevelID = LEVEL::TOOLS;

		LoadFile.read(reinterpret_cast<_char*>(&iLoadLength), sizeof(_int));
		tDesc.strName.resize(iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_tchar) * iLoadLength);
		LoadFile.read(reinterpret_cast<_char*>(&WorldMatrix), sizeof(_float4x4));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fSpeedPerSec), sizeof(_float));
		LoadFile.read(reinterpret_cast<_char*>(&tDesc.fRotationPerSec), sizeof(_float));

		tDesc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_") + tDesc.strName,
			ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Monster"), &tDesc)))
			return E_FAIL;

		Add_Modify_ListBox(m_MonsterNames, tDesc.strName);
	}

	LoadFile.close();
	MSG_BOX("파일 로드 성공");

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

	m_ProtoEnvironmentNames.clear();
	m_ProtoItemNames.clear();
	m_ProtoMonsterNames.clear();

	m_EnvironmentNames.clear();
	m_ChestNames.clear();
	m_MonsterNames.clear();

	Safe_Release(m_pModifyObject);
	Safe_Release(m_pMap);
}
