#include "Cell.h"

#include "GameInstance.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	_vector		vLine = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	/* 각 라인의 노말 구하기. 원소를 바꾸고 -1을 곱해주면 법선벡터가 나옴  */
	/* y는 굳이 계산 해 줄 필요가 없어서 그냥 0 넣어줌 */
	vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	m_vNormals[LINE_AB] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);

	vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	m_vNormals[LINE_BC] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);

	vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	m_vNormals[LINE_CA] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);


	return S_OK;
}

_bool CCell::isIn(_fvector vLocalPos, _int* pNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; i++)
	{
		/* 객체의 위치에서 삼각형 점의 위치를 빼서 방향벡터를 구한다음
		( vLocalPos가 위치값이기 때문에 원래는 w를 벡터셋으로 날려줘야함 )
		하지만 Float3로 연산을 하기때문에 w는 자연스럽게 사라질 것 */
		_vector		vDir = vLocalPos - XMLoadFloat3(&m_vPoints[i]);

		/* 구한 법선벡터와 내적을해 양수면 바깥, 음수면 안쪽 */
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])))))
		{
			if(nullptr != pNeighborIndex)
				*pNeighborIndex = m_iNeighborIndices[i];

			return false;
		}
	}

	return true;
}

_bool CCell::Compare(_fvector vSour, _fvector vDest)
{
	/*XMVectorEqual(vSour, vDest);*/ // 각 요소끼리 비교해서 벡터를 반환하기때문에 3를 쓰자

	/* 인자로받은 점의 위치와 본인의 위치가 같은 위치에 해당되면 true를 반환해서 이웃임을 알려줌 */
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDest))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDest))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDest))
			return true;
	}

	return false;
}

_float CCell::Compute_Height(_fvector vLocalPos)
{
	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));

	/* 평면의 방정식 : y = (-ax - cz - d) / b */
	return (-vPlane.m128_f32[0] * vLocalPos.m128_f32[0] - vPlane.m128_f32[2] * vLocalPos.m128_f32[2] - vPlane.m128_f32[3]) / vPlane.m128_f32[1];
}

_vector CCell::Compute_SlidingVector(_fvector vLocalPos, _fvector vMovePoint)
{
	_vector vMoveDir = vMovePoint - vLocalPos;
	_vector vSlideLocal = vMovePoint;

	for (size_t i = 0; i < LINE_END; ++i)
	{
		_vector vToEdge = XMVectorSetY(vMovePoint - XMLoadFloat3(&m_vPoints[i]), 0.f);
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vToEdge), vNormal)))
		{
			// 슬라이딩 방향 계산
			_vector vSlideDir = vMoveDir - XMVector3Dot(vMoveDir, vNormal) * vNormal;

			// 슬라이딩 위치 계산 (로컬 공간에서)
			vSlideLocal = vLocalPos + vSlideDir;
		}
	}

	return vSlideLocal;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}
#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif
}
