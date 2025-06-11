#include "NavigationTool.h"
#include "GameInstance.h"

#include "Map.h"

#include "Cell.h"

CNavigationTool::CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool{ pDevice, pContext }
{
}

void CNavigationTool::Set_Map(CMap* pMap)
{
	if (nullptr == pMap)
		return;

	if (nullptr != m_pMap)
		Safe_Release(m_pMap);

	m_pMap = pMap;
	Safe_AddRef(m_pMap);
}

HRESULT CNavigationTool::Initialize()
{
	m_bMode[NORMAL] = true;

	return S_OK;
}

void CNavigationTool::Update(_float fTimeDelta)
{
	Key_Input();
	if (nullptr == m_pMap || m_bMode[NORMAL])
		return;

	if (m_bMode[CREATE])
	{
		if (MOUSE_DOWN(DIMK::LBUTTON))
		{
			_float3 vInitPos = m_pMap->Get_PickedPos_World();
			XMStoreFloat3(&vInitPos, XMVectorRound(XMLoadFloat3(&vInitPos)));

			cout << "X : " << vInitPos.x << "Y : " << vInitPos.y << "Z : " << vInitPos.z << '\n';
		}
	}

}

HRESULT CNavigationTool::Render()
{
	Render_UI();
	return S_OK;
}

HRESULT CNavigationTool::Render_ExtraUI()
{
	return S_OK;
}

void CNavigationTool::Key_Input()
{
	if (KEY_DOWN(DIK_V))
	{
		m_bMode[CREATE] = !m_bMode[CREATE];
		m_bMode[NORMAL] = !m_bMode[NORMAL];
	}
}

void CNavigationTool::Change_Mode()
{
	if (ImGui::RadioButton(u8"생성", m_bMode[CREATE]))
	{
		m_bMode[CREATE] = !m_bMode[CREATE];
		m_bMode[NORMAL] = false;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"잠금", m_bMode[NORMAL]))
	{
		m_bMode[NORMAL] = !m_bMode[NORMAL];
		m_bMode[CREATE] = false;
	}

	ImGui::Text(u8"키를 눌러 모드 전환");
	ImGui::Text(u8"V : 생성/잠금 ");
}

void CNavigationTool::Add_ClickedPoint(_float3 vWorldPos)
{
	if (3 <= m_ClickedPoints.size())
		return;

	m_ClickedPoints.push_back(vWorldPos);
	m_vClickedPos = vWorldPos;

	if (3 == m_ClickedPoints.size())
	{

		m_ClickedPoints.clear();
	}


}

HRESULT CNavigationTool::Create_Cell()
{
	return S_OK;
}

HRESULT CNavigationTool::Render_UI()
{
	ImGui::Begin(u8"네비게이션 툴");
	Check_SelectedTool();


	Change_Mode();






	ImGui::End();
	return S_OK;
}

CNavigationTool* CNavigationTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavigationTool* pInstance = new CNavigationTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CNavigationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigationTool::Free()
{
	__super::Free();

	Safe_Release(m_pMap);
}
