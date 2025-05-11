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
	/* �ٷ� 11������ ���� �ڷ����� ������ */
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

protected:
	/* ���߿� ������ ��ġ�� �˱� ���� ���� */
	_float3* m_pVertexPositions = {};

protected:
	/* ���� ���� */
	_uint						m_iNumVertices = {};
	/* ������ ũ�� */
	_uint						m_iVertexStride = {};
	/* �ε��� ���� */
	_uint						m_iNumIndices = {};
	/* �ε����� ũ��*/
	_uint						m_iIndexStride = {};
	/* ���ؽ� ������ ���� */
	_uint						m_iNumVertexBuffers = {};
	/* �ε��� ���� */
	DXGI_FORMAT					m_eIndexFormat = {};
	/* ������Ƽ�� �������� (Ʈ���̾ޱ� ����Ʈ �� ) */
	D3D11_PRIMITIVE_TOPOLOGY	m_ePrimitiveTopology = {};
	void*						m_pIndices = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END