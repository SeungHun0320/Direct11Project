#include "MapTool.h"
#include "GameInstance.h"

#include "Courtyard.h"
#include "Arena.h"



CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{pDevice, pContext}
{
}

HRESULT CMapTool::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CMapTool::Update(_float fTimeDelta)
{
}

HRESULT CMapTool::Render()
{
	MapTool();

	return S_OK;
}

HRESULT CMapTool::MapTool()
{
	ImGui::Begin(u8"¸Ê Åø");

	Map_Menu();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	if (ImGui::BeginTabBar(u8"¸ðµ¨ ¼±ÅÃ"))
	{
		if (ImGui::BeginTabItem(u8"È¯°æ"))
		{
			Environment_ListBox();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"Àû"))
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

	if (ImGui::BeginCombo(u8"¸Ê ¼±ÅÃ", eCurrntMap == MAP_END ? u8"¼±ÅÃ" : szMaps[eCurrntMap]))
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
			MSG_BOX("¸Ê »ý¼º ½ÇÆÐ,,");
			return;
		}
			
		ePreMap = eCurrntMap;
	}
}

HRESULT CMapTool::Craete_Map(MAP iMapIdx, const _wstring& strLayerTag)
{
	_wstring strPrototypeTag;

	switch (iMapIdx)
	{
	case MAP::COURTYARD:
		m_strMapFileTag = TEXT("Courtyard.Map");
		strPrototypeTag = TEXT("Prototype_GameObject_Courtyard");
		break;

	case MAP::MAIN:
		m_strMapFileTag = TEXT("Main.Map");
		strPrototypeTag = TEXT("Prototype_GameObject_Main");
		break;

	case MAP::ARENA:
		m_strMapFileTag = TEXT("Arena.Map");
		strPrototypeTag = TEXT("Prototype_GameObject_Arena");
		break;
	default:
		break;
	}

	CMap* pMap = static_cast<CMap*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::TOOLS), strLayerTag));
	if (nullptr != pMap)
		pMap->Set_Dead(true);

	CMap::DESC tDesc = {};
	tDesc.eLevelID = LEVEL::TOOLS;
	tDesc.fRotationPerSec = 0.f;
	tDesc.fSpeedPerSec = 0.f;

	if(FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), strPrototypeTag,
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapTool::Environment_ListBox()
{
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

void CMapTool::Save()
{
}

void CMapTool::Load()
{
}

HRESULT CMapTool::Save_Map(const _wstring& strMapTag)
{
	return E_NOTIMPL;
}

HRESULT CMapTool::Load_Map(const _wstring& strMapTag)
{
	return E_NOTIMPL;
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
}
