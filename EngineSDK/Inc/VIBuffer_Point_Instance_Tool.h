#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance_Tool final : public CVIBuffer_Instance
{
public:
	typedef struct tagPointInstanceDesc : public CVIBuffer_Instance::DESC
	{
		_float3 vPivot{};
		_float2 vLifeTime{};
		_float2 vSpeed{};
		_bool   isLoop{};

	}DESC;

private:
	CVIBuffer_Point_Instance_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance_Tool(const CVIBuffer_Point_Instance_Tool& Prototype);
	virtual ~CVIBuffer_Point_Instance_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype(const DESC* pArg);
	virtual HRESULT Initialize(void* pArg);

	virtual void Drop(_float fTimeDelta)override;
	virtual void Spread(_float fTimeDelta)override;


private:
	VTXPOINT_PARTICLE_INSTANCE* m_pVertexInstances = { nullptr };
	_float* m_pSpeeds = { nullptr };
	/* 파티클이 생성되는 시작점? */
	_float3					    m_vPivot = {};
	_bool                       m_isLoop = { false };


public:
	static CVIBuffer_Point_Instance_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const DESC* pArg);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

END