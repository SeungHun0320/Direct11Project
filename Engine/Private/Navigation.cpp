#include "Navigation.h"

#include "Cell.h"
#include "GameInstance.h"


_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext }
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CNavigation::CNavigation(const CNavigation& Prototype)
    : CComponent(Prototype)
    , m_Cells { Prototype.m_Cells }
#ifdef _DEBUG
    , m_pShader{ Prototype.m_pShader }
#endif
{
    for (auto& pCell : m_Cells)
        Safe_AddRef(pCell);

#ifdef _DEBUG
    Safe_AddRef(m_pShader);
#endif

}

HRESULT CNavigation::Initialize_Prototype(const _wstring& strNavigationFilePath)
{
#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	// 파일 입출력해서 읽어오기

	//CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
	//if (nullptr == pCell)
	//	return E_FAIL;

	//m_Cells.push_back(pCell);


	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

    return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* 어디 셀에서 시작할지 정함 */
	m_iIndex = pDesc->iIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	/* 오직 맵에서만 월드매트릭스를 저장시켜줌 (스태틱으로 둔 이유 )*/
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

_bool CNavigation::isMove(_fvector vWorldPos)
{
	/* 받아온 맵(지형의) 월드 매트릭스의 역행렬을 곱해서, 월드에 있는 객체의 위치를 로컬로 땡겨온다. */
	_vector		vLocalPos = XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	/* 초기 이웃의 인덱스 -1이면 이웃이 없다는 뜻 */
	_int		iNeighborIndex = { -1 };

	/* 현재 본인이 서있는 인덱스에 해당되는 삼각형 위에 서있다면? */
	if (true == m_Cells[m_iIndex]->isIn(vLocalPos, &iNeighborIndex))
		return true;
	else
	{
		/* 이웃이 없다면 */
		if (-1 == iNeighborIndex)
			return false;
		else
		{
			/* 이웃이 있다면 */
			while (true)
			{
				/* 네비메쉬를 타는 객체와 이웃끼리 겹쳐있을 때의 예외처리  */
				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
					break;

				/* 없으면 false */
				if (-1 == iNeighborIndex)
					return false;
			}

			/* 현재 서 있는 인덱스를 변경 */
			m_iIndex = iNeighborIndex;
			return true;
		}
	}
}

_vector CNavigation::SetUp_Hegiht(_fvector vWorldPos)
{
	/* 로컬로 변환 후에 */
	_vector		vLocalPos = XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	/* 셀의 높이값을 갖고와서 적용 후*/
	vLocalPos = XMVectorSetY(vLocalPos, m_Cells[m_iIndex]->Compute_Height(vLocalPos));

	/* 다시 월드로 변환해서 리턴 */
	return XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix));
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	/* 그리기를 그냥 월드에서 하고 처리는 로컬로 옮겨서 하자. 셀이 많아질수록 매 프레임 연산이 엄청나게 많아지기 때문에 */
	/* 아싸리 네비게이션 위에 있는 객체를 로컬로 땡겨와서 연산을 해주는게 이득임 */
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ));

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
		pCell->Render();

	return S_OK;
}
#endif

HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strNavigationFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strNavigationFilePath)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
    __super::Free();

    for (auto& pCell : m_Cells)
        Safe_Release(pCell);

#ifdef _DEBUG
    Safe_Release(m_pShader);
#endif
}
