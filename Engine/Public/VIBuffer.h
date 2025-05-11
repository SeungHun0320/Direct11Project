#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
	protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	virtual HRESULT Bind_Buffers();
	virtual HRESULT Render();
public:
	virtual _float3 Compute_HeightPosition(const _float3& vPosition) { return vPosition; }
	_float3 Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse);
	_float3 Compute_PickedPosition_World(const _float4x4* pWorldMatrix);
	_float3 Compute_PickedPosition_World_Snap(const _float4x4* pWorldMatrix);

protected:
	/* 다렉 11에서는 버퍼 자료형이 같아짐 */
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

protected:
	/* 나중에 정점의 위치를 알기 위한 정보 */
	_float3* m_pVertexPositions = {};

protected:
	/* 정점 갯수 */
	_uint						m_iNumVertices = {};
	/* 정점의 크기 */
	_uint						m_iVertexStride = {};
	/* 인덱스 갯수 */
	_uint						m_iNumIndices = {};
	/* 인덱스의 크기*/
	_uint						m_iIndexStride = {};
	/* 버텍스 버퍼의 갯수 */
	_uint						m_iNumVertexBuffers = {};
	/* 인덱스 포맷 */
	DXGI_FORMAT					m_eIndexFormat = {};
	/* 프리미티브 토폴로지 (트라이앵글 리스트 등 ) */
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveTopology = {};
	void*						m_pIndices = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END