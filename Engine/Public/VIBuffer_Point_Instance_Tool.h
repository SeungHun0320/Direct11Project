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

		_float2 vFrameXY{};
		_float2 vFrameSpeed{};
	}DESC;

private:
	CVIBuffer_Point_Instance_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance_Tool(const CVIBuffer_Point_Instance_Tool& Prototype);
	virtual ~CVIBuffer_Point_Instance_Tool() = default;
public:
	void Set_NumInstnace(_uint iNumInstance);
	void Set_Size(_float2 vSize);
	void Set_Pivot(_float3 vPivot);
	void Set_Translation(_float3 vCenter, _float3 vRange);
	void Set_LifeTime(_float2 vLifeTime);
	void Set_Speed(_float2 vSpeed);
	void Set_isLoop(_bool isLoop);
	void Set_SpriteSpeed(_float2 vSpeed);
	void Set_FrameXY(_float2 vFrameXY);
	void Set_Scale(_float2 vScale);

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