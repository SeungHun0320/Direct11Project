#include "Tool_Navigation.h"
#include "GameInstance.h"

#include "Map.h"

#include "Cell.h"

CTool_Navigation::CTool_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool{ pDevice, pContext }
{
}

void CTool_Navigation::Set_Map(CMap* pMap)
{
	if (nullptr == pMap)
		return;

	if (nullptr != m_pMap)
		Safe_Release(m_pMap);

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

	m_pMap = pMap;
	Safe_AddRef(m_pMap);

	if (L"Courtyard" == m_pMap->Get_Name())
	{
		m_strNaviFileTag = "Courtyard.Nav";
	}
	else if (L"Arena" == m_pMap->Get_Name())
	{
		m_strNaviFileTag = "Arena.Nav";
	}
	else if (L"Shop" == m_pMap->Get_Name())
	{
		m_strNaviFileTag = "Shop.Nav";
	}
}

HRESULT CTool_Navigation::Initialize()
{
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_ClickedPoints.reserve(3);

	m_bMode[MODIFY] = true;

	return S_OK;
}

void CTool_Navigation::Update(_float fTimeDelta)
{
	Key_Input();
	if (nullptr == m_pMap || m_bMode[NORMAL])
		return;

	m_vCurrentPos = m_pMap->Get_PickedPos_World();

	if (m_bMode[CREATE])
	{
		if (MOUSE_DOWN(DIMK::LBUTTON))
			Add_ClickedPoint(m_vCurrentPos);
	}

	if (m_bMode[MODIFY])
	{
		if (MOUSE_DOWN(DIMK::RBUTTON))
			Delete_Cell(XMLoadFloat3(&m_vCurrentPos));
	}

}

HRESULT CTool_Navigation::Render()
{
	Render_UI();

	if (FAILED(Render_Cells()))
		return E_FAIL;

	return S_OK;

}

HRESULT CTool_Navigation::Render_ExtraUI()
{
	return S_OK;
}

void CTool_Navigation::Key_Input()
{
	if (KEY_DOWN(DIK_V))
	{
		m_bMode[CREATE] = !m_bMode[CREATE];
		m_bMode[MODIFY] = !m_bMode[MODIFY];
	}
	if (KEY_DOWN(DIK_B))
		m_bMode[NORMAL] = !m_bMode[NORMAL];
}

void CTool_Navigation::Change_Mode()
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

HRESULT CTool_Navigation::Render_Cells()
{
	if (nullptr == m_pMap)
		return E_FAIL;

	_float4x4		WorldMatrix = *dynamic_cast<CTransform*>(m_pMap->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix_Float4x4();
	WorldMatrix.m[3][1] += 0.2f;

	m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ));

	m_pShader->Begin(0);

#ifdef _DEBUG
	for (auto& pCell : m_Cells)
		pCell->Render();
#endif 

	return S_OK;
}

void CTool_Navigation::Add_ClickedPoint(_float3 vWorldPos)
{
	if (3 <= m_ClickedPoints.size())
		return;

	_float3 vSnapPos = Snap_NearCellPoint(vWorldPos);

	m_ClickedPoints.push_back(vSnapPos);
	m_vClickedPos = vSnapPos;

	if (3 == m_ClickedPoints.size())
	{
		Sort_PointsCW(m_ClickedPoints);

		if(!Check_Cells(m_ClickedPoints))
			Create_Cell();

		m_ClickedPoints.clear();
	}
}

HRESULT CTool_Navigation::Create_Cell()
{
	_float3 vPoints[3] = {
		m_ClickedPoints[0],
		m_ClickedPoints[1],
		m_ClickedPoints[2],
	};

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, static_cast<_int>(m_Cells.size()));
	if (nullptr == pCell)
	{
		MSG_BOX("셀을 못만들었데용");
		return E_FAIL;
	}

	m_Cells.push_back(pCell);

	return S_OK;
}

CCell* CTool_Navigation::Find_Cell(_fvector vPickedPos)
{
	for (auto& pCell : m_Cells)
	{
		_int iNeighborIdex{ -1 };
		if (pCell->isIn(vPickedPos, &iNeighborIdex))
			return pCell;
	}

	return nullptr;
}

void CTool_Navigation::Delete_Cell(_fvector vPickedPos)
{
	for (auto iter = m_Cells.begin(); iter != m_Cells.end();)
	{
		_int iNeighborIdex{ -1 };
		if ((*iter)->isIn(vPickedPos, &iNeighborIdex))
		{
			Safe_Release(*iter);
			m_Cells.erase(iter);
			break;
		}
		else
			++iter;
	}
}

_float3 CTool_Navigation::Snap_NearCellPoint(const _float3& vPickedPos)
{
	if (m_Cells.empty())
		return vPickedPos;

	_float3 vSnappedPos = vPickedPos;
	_float fMinDist = { FLT_MAX };

	for (auto& pCell : m_Cells)
	{
		for (_uint i = 0; i < CCell::POINT_END; i++)
		{
			_vector vPoint = pCell->Get_Point(static_cast<CCell::POINT>(i));

			_float fDist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPickedPos) - vPoint));

			if (fDist < fMinDist && fDist < m_fSnapRange)
			{
				fMinDist = fDist;
				XMStoreFloat3(&vSnappedPos, vPoint);
			}
		}
	}
	return vSnappedPos;
}

void CTool_Navigation::Sort_PointsCW(vector<_float3>& Points)
{
	if (Points.size() != 3)
		return;

	_vector v0 = XMLoadFloat3(&Points[0]);
	_vector v1 = XMLoadFloat3(&Points[1]);
	_vector v2 = XMLoadFloat3(&Points[2]);

	// 벡터 계산
	_vector vA = v1 - v0;
	_vector vB = v2 - v0;

	// 외적 계산
	_vector vNormal = XMVector3Cross(vA, vB);

	if (0.f > XMVectorGetY(vNormal))
	{
		swap(Points[0], Points[2]);
	}
}

_bool CTool_Navigation::Check_Cells(vector<_float3>& Points)
{
	if (m_Cells.empty())
		return false;

	for (auto& pCell : m_Cells)
	{
		_float3 vCellPoints[3] = {};

		XMStoreFloat3(&vCellPoints[0], pCell->Get_Point(CCell::POINT_A));
		XMStoreFloat3(&vCellPoints[1], pCell->Get_Point(CCell::POINT_B));
		XMStoreFloat3(&vCellPoints[2], pCell->Get_Point(CCell::POINT_C));

		_uint iMatchCount = 0;
		for (_uint i = 0; i < CCell::POINT_END; i++)
		{
			for (_uint j = 0; j < CCell::POINT_END; j++)
			{
				if (XMVector3Equal(XMLoadFloat3(&Points[i]), XMLoadFloat3(&vCellPoints[j])))
					++iMatchCount;
			}
		}

		if (3 == iMatchCount)
			return true;
	}

	return false;
}

void CTool_Navigation::Save_Load_Menu()
{
	if (ImGui::Button(u8"저장"))
	{
		Save_Navigation("../bin/DataFiles/NavigationData/" + m_strNaviFileTag);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"불러오기"))
	{
		Load_Navigation("../bin/DataFiles/NavigationData/" + m_strNaviFileTag);
	}
}

HRESULT CTool_Navigation::Save_Navigation(const _string& strNaviFileTag)
{
	ofstream OutFile(strNaviFileTag, std::ios::binary);

	if (!OutFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	_uint iNumCells = static_cast<_uint>(m_Cells.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumCells), sizeof(_uint));

	_matrix MapWorldMatrix = dynamic_cast<CTransform*>(m_pMap->Get_Component(TEXT("Com_Transform")))->Get_WorldMatrix();
	_matrix MapWorldMatrixInverse = XMMatrixInverse(nullptr, MapWorldMatrix);

	for (auto& pCell : m_Cells)
	{
		_float3 vPoints[3] = {};
		for (_uint i = 0; i < 3; i++)
		{
			_vector vWorldPos = pCell->Get_Point(static_cast<CCell::POINT>(i));
			_vector vLocalPos = XMVector3TransformCoord(vWorldPos, MapWorldMatrixInverse);
			XMStoreFloat3(&vPoints[i], vLocalPos);
		}

		OutFile.write(reinterpret_cast<const _char*>(vPoints), sizeof(_float3) * 3);
	}

	OutFile.close();

	MSG_BOX("셀 저장 성공!");
	return S_OK;
}

HRESULT CTool_Navigation::Load_Navigation(const _string& strNaviFileTag)
{
	std::ifstream InFile(strNaviFileTag, std::ios::binary);
	if (!InFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");

		return E_FAIL;
	}

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

	_uint iNumCells{};
	InFile.read(reinterpret_cast<_char*>(&iNumCells), sizeof(_uint));

	for (_uint i = 0; i < iNumCells; ++i)
	{
		_float3 vPoints[3] = {};
		InFile.read(reinterpret_cast<_char*>(vPoints), sizeof(_float3) * 3);

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, static_cast<_int>(m_Cells.size()));
		if (nullptr == pCell)
		{
			MSG_BOX("셀 생성 실패,,");
			return E_FAIL;
		}

		m_Cells.push_back(pCell);
	}

	InFile.close();

	MSG_BOX("셀 불러오기 성공!");
	return S_OK;
}

HRESULT CTool_Navigation::Render_UI()
{
	ImGui::Begin(u8"네비게이션 툴");
	Check_SelectedTool();
	Change_Mode();

	ImGui::Text(u8"현재 마우스 위치 :");
	ImGui::SameLine();
	ImGui::InputFloat3("##PointA", reinterpret_cast<_float*>(&m_vCurrentPos));

	Save_Load_Menu();


	ImGui::End();
	return S_OK;
}

CTool_Navigation* CTool_Navigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Navigation* pInstance = new CTool_Navigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CTool_Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Navigation::Free()
{
	__super::Free();

	Safe_Release(m_pMap);
	Safe_Release(m_pShader);

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
}
