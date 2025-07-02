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
		_float2		vSize;
		_float3		vRange;
		_float3		vCenter;
	
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

	/* 가상함수로 둬서 캐스팅 안하고 쓸 수 있도록 했음 */
	virtual void Drop(_float fTimeDelta) {};
	virtual void Spread(_float fTimeDelta) {};
	virtual void Sprite(_float fTimeDelta) {};
	virtual void MoveTrail(_fvector vWorldPos, _float fTimeDelta) {};
	virtual void Shrink(_float fTimeDelta) {};
	virtual void Reset() {};

protected:
	/* 인스턴스용 버퍼 */
	ID3D11Buffer*			m_pVBInstance = { nullptr };
	/* 인스턴스 버퍼를 할당할때 필요한 정보들, 멤버변수로 들고있다가 이니셜라이즈에서 생성시켜줌 */
	/* 할당 두 번하는거 싫어서 */
	D3D11_BUFFER_DESC		m_VBInstanceDesc = {};
	D3D11_SUBRESOURCE_DATA  m_VBInstanceSubResourceData = {};

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