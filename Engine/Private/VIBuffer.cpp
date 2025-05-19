#include "VIBuffer.h"
#include "GameInstance.h"
CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }

{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent( Prototype )
	, m_pVB{ Prototype.m_pVB }
	, m_pIB{ Prototype.m_pIB }
	, m_iNumVertices{ Prototype.m_iNumVertices }
	, m_iVertexStride{ Prototype.m_iVertexStride }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_iIndexStride{ Prototype.m_iIndexStride }
	, m_pVertexPositions{ Prototype.m_pVertexPositions }
	, m_iNumVertexBuffers{ Prototype.m_iNumVertexBuffers }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_ePrimitiveTopology{ Prototype.m_ePrimitiveTopology }
	, m_pIndices{ Prototype.m_pIndices }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		/*m_pVB1,
		m_pVB2,*/
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride,
	};

	_uint		iOffsets[] = {
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	/*LPCSTR SemanticName;
	UINT SemanticIndex;
	DXGI_FORMAT Format;
	UINT InputSlot;
	UINT AlignedByteOffset;
	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	UINT InstanceDataStepRate;
	*/

	//D3D11_INPUT_ELEMENT_DESC		Elements[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//};

	//_uint		iNumElements = 2;

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

_float3 CVIBuffer::Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	/* 컴퓨트 포지션을 호출 할 때 객체의 월드매트릭스의 역행렬을 받도록 해서, */
	/* 마우스 좌표를 로컬로 바꿔준 후에 피킹함수를 호출 해 준다 */
	m_pGameInstance->Transform_Picking_ToLocalSpace(WorldMatrixInverse);

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		if (true == m_pGameInstance->Picking_InLocal(vPickedPos, m_pVertexPositions[iIndices[0]], m_pVertexPositions[iIndices[1]], m_pVertexPositions[iIndices[2]]))
			break;
	}

	return vPickedPos;
}

_float3 CVIBuffer::Compute_PickedPosition_World(const _float4x4* pWorldMatrix)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		_float3 v0, v1, v2;
		XMStoreFloat3(&v0, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[0]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v1, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[1]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v2, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[2]]), XMLoadFloat4x4(pWorldMatrix)));

		if (true == m_pGameInstance->Picking_InWorld(vPickedPos, v0, v1, v2))
			break;
	}

	return vPickedPos;
}

_float3 CVIBuffer::Compute_PickedPosition_World_Snap(const _float4x4* pWorldMatrix)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		_float3 v0, v1, v2;
		/* 받아온 월드행렬로 정점의 위치를 월드로 변환 */
		XMStoreFloat3(&v0, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[0]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v1, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[1]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v2, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[2]]), XMLoadFloat4x4(pWorldMatrix)));

		/* 현재 y값은 정렬에서 제외했음 */
		if (true == m_pGameInstance->Picking_InWorld(vPickedPos, v0, v1, v2))
		{
			_float fMinDistSq = FLT_MAX;
			_float3 vSnapedPos = {};

			_float3 vTest[3] = { v0, v1, v2 };
			for (_uint j = 0; j < 3; ++j)
			{
				_float dx = vTest[j].x - vPickedPos.x;
				_float dz = vTest[j].z - vPickedPos.z;

				_float fDistSq = dx * dx + dz * dz;

				if (fDistSq < fMinDistSq)
				{
					fMinDistSq = fDistSq;
					vSnapedPos = vTest[j];
				}
			}

			vPickedPos.x = vSnapedPos.x;
			vPickedPos.z = vSnapedPos.z;
			break;
		}
	}

	return vPickedPos;
}

_float3 CVIBuffer::Compute_PickedPosition_LocalEx(_fmatrix WorldMatrixInverse, _float& fDist)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};

	/* 컴퓨트 포지션을 호출 할 때 객체의 월드매트릭스의 역행렬을 받도록 해서, */
	/* 마우스 좌표를 로컬로 바꿔준 후에 피킹함수를 호출 해 준다 */
	m_pGameInstance->Transform_Picking_ToLocalSpace(WorldMatrixInverse);

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		if (true == m_pGameInstance->Picking_InLocalEx(vPickedPos, fDist, m_pVertexPositions[iIndices[0]], m_pVertexPositions[iIndices[1]], m_pVertexPositions[iIndices[2]]))
			break;
	}

	return vPickedPos;
}

_float3 CVIBuffer::Compute_PickedPosition_WorldEx(const _float4x4* pWorldMatrix, _float& fDist)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};
	_bool   bHit = { false };
	_float fMinDist = { FLT_MAX };

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		_float3 v0, v1, v2;
		XMStoreFloat3(&v0, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[0]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v1, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[1]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v2, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[2]]), XMLoadFloat4x4(pWorldMatrix)));

		_float3 vPickedCheckPos = {};

		if (true == m_pGameInstance->Picking_InWorldEx(vPickedCheckPos, fDist, v0, v1, v2))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				vPickedPos = vPickedCheckPos;
				bHit = true;
			}
		}

	}

	if (bHit)
	{
		return vPickedPos;
	}
	else
	{
		fDist = FLT_MAX;
		return _float3(0.f, 0.f, 0.f);
	}
		
}

_float3 CVIBuffer::Compute_PickedPosition_World_SnapEx(const _float4x4* pWorldMatrix, _float& fDist)
{
	_uint   iIndices[3] = {};
	_float3 vPickedPos = {};
	_float fMinDist = { FLT_MAX };
	_bool   bHit = { false };

	_uint iNumPrimitive = static_cast<_uint>(m_iNumIndices / 3.f);

	for (_uint i = 0; i < iNumPrimitive; i++)
	{
		_byte* pIndices = static_cast<_byte*>(m_pIndices) + m_iIndexStride * i * 3;

		memcpy(&iIndices[0], pIndices, m_iIndexStride);
		memcpy(&iIndices[1], pIndices + m_iIndexStride, m_iIndexStride);
		memcpy(&iIndices[2], pIndices + m_iIndexStride * 2, m_iIndexStride);

		_float3 v0, v1, v2;
		/* 받아온 월드행렬로 정점의 위치를 월드로 변환 */
		XMStoreFloat3(&v0, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[0]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v1, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[1]]), XMLoadFloat4x4(pWorldMatrix)));
		XMStoreFloat3(&v2, XMVector3TransformCoord(XMLoadFloat3(&m_pVertexPositions[iIndices[2]]), XMLoadFloat4x4(pWorldMatrix)));

		/* 현재 y값은 정렬에서 제외했음 */
		if (true == m_pGameInstance->Picking_InWorldEx(vPickedPos, fDist, v0, v1, v2))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				bHit = true;

				_float fMinDistSq = FLT_MAX;
				_float3 vSnapedPos = {};

				_float3 vTest[3] = { v0, v1, v2 };
				for (_uint j = 0; j < 3; ++j)
				{
					_float dx = vTest[j].x - vPickedPos.x;
					_float dz = vTest[j].z - vPickedPos.z;

					_float fDistSq = dx * dx + dz * dz;

					if (fDistSq < fMinDistSq)
					{
						fMinDistSq = fDistSq;
						vSnapedPos = vTest[j];
					}
				}

				vPickedPos.x = vSnapedPos.x;
				vPickedPos.z = vSnapedPos.z;
			}
		}
	}

	if (bHit)
	{
		return vPickedPos;
	}
	else
	{
		fDist = FLT_MAX;
		return _float3(0.f, 0.f, 0.f);
	}
}


void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertexPositions);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
