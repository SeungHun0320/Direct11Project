#pragma once

#include "VIBuffer.h"

#define TRAIL_COUNT 30

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
private:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& Prototype);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	void Update_Trail(const _float3& vLeft, const _float3& vRight);
	void Update_VertexBuffer();

private:
	_float3 m_vLeftPoints[TRAIL_COUNT] = {};
	_float3 m_vRightPoints[TRAIL_COUNT] = {};

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END