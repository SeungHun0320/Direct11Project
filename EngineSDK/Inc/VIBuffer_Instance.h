#pragma once

#include "VIBuffer.h"

/*
인스턴싱이 필요한 모든 모델들의 부모가 된다.
*/

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_uint		iNumInstance;
    }DESC;

protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& Prototype);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

protected:
	/* 인스턴스용 버퍼 */
	ID3D11Buffer*			m_pVBInstance = { nullptr };
	/* 버퍼를 할당할때 필요한 정보들, 멤버변수로 들고있음 */
	D3D11_BUFFER_DESC		m_VBInstanceDesc = {};
	D3D11_SUBRESOURCE_DATA  m_VBInstanceSubResourceData = {};
	VTXMATRIX*				m_pVertexInstances = { nullptr };

	/* 몇 번째인덱스까지 정점버퍼를 쓸건지? */
	_uint					m_iNumIndexPerInstance = {};
	/* 인스턴스버퍼 갯수 */
	_uint					m_iNumInstance = {};
	/* 인스턴스버퍼의 구조체 크기 */
	_uint					m_iVertexInstanceStride = {};

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END