#include "VIBuffer_Trail.h"
#include "GameInstance.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer{pDevice, pContext}
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& Prototype)
    : CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = TRAIL_COUNT * 2;
	m_iNumIndices = (TRAIL_COUNT - 1) * 2 * 3;
	m_iIndexStride = sizeof(_ushort);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC VBBufferDesc{};
	VBBufferDesc.ByteWidth = sizeof(VTXPOSTEX) * m_iNumVertices;
	VBBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, nullptr, &m_pVB)))
		return E_FAIL;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	_int iNumIndices = 0;
	for (_uint i = 0; i < TRAIL_COUNT - 1; ++i)
	{
		_uint iBase = i * 2;
		pIndices[iNumIndices++] = iBase;
		pIndices[iNumIndices++] = iBase + 2;
		pIndices[iNumIndices++] = iBase + 1;

		pIndices[iNumIndices++] = iBase + 1;
		pIndices[iNumIndices++] = iBase + 2;
		pIndices[iNumIndices++] = iBase + 3;
	}

	D3D11_BUFFER_DESC IBBufferDesc{};
	IBBufferDesc.ByteWidth = sizeof(_ushort) * m_iNumIndices;
	IBBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

    return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
    return S_OK;
}

void CVIBuffer_Trail::Update_Trail(const _float3& vLeft, const _float3& vRight)
{
	for (_uint i = TRAIL_COUNT - 1; i > 0; --i)
	{
		m_vLeftPoints[i] = m_vLeftPoints[i - 1];
		m_vRightPoints[i] = m_vRightPoints[i - 1];
	}

	m_vLeftPoints[0] = vLeft;
	m_vRightPoints[0] = vRight;
}

void CVIBuffer_Trail::Update_VertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE SubResorce{};
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResorce)))
		return;

	VTXPOSTEX* pVertices = static_cast<VTXPOSTEX*>(SubResorce.pData);

	for (_uint i = 0; i < TRAIL_COUNT; ++i)
	{
		_float fUVY = static_cast<_float>(i / (TRAIL_COUNT - 1.f));
		pVertices[i * 2 + 0].vPosition = m_vLeftPoints[i];
		pVertices[i * 2 + 0].vTexcoord = _float2(0.f, fUVY);

		pVertices[i * 2 + 1].vPosition = m_vRightPoints[i];
		pVertices[i * 2 + 1].vTexcoord = _float2(1.f, fUVY);
	}

	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}
