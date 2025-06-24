#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final: public CVIBuffer_Instance
{
public:
	typedef struct tagRectInstance : public CVIBuffer_Instance::DESC
	{
		_float2		vLifeTime;
		_float2		vSpeed;
		_bool		isLoop;

	}DESC;

private:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& Prototype);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const DESC* pArg);
	virtual HRESULT Initialize(void* pArg);

	virtual void Drop(_float fTimeDelta) override;
	virtual void Spread(_float fTimeDelta) override;

private:
	VTXRECT_PARTICLE_INSTANCE* m_pVertexInstances = { nullptr };
	/* 스피드를 입자마다 따로 설정해주기 위해서 포인터로 담은뒤 동적할당해서 사용 */
	_float*					   m_pSpeeds = { nullptr };
	_bool					   m_isLoop = { false };

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const DESC* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END