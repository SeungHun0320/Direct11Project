#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagPointInstanceDesc : public CVIBuffer_Instance::DESC
	{
		_float3 vPivot{};
		_float2 vLifeTime{};
		_float2 vSpeed{};

		_float2 vFrameXY{};
		_float2 vFrameSpeed{};
		_bool   isLoop{};

		_float2 vRotationZ{};
	}DESC;

private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const DESC* pArg);
	virtual HRESULT Initialize_Prototype(const _wstring& strParticleFilePath);
	virtual HRESULT Initialize(void* pArg);

	virtual void Reset() override;
	virtual void Drop(_float fTimeDelta) override;
	virtual void Spread(_float fTimeDelta) override;
	virtual void MoveTrail(_fvector vWorldPos, _float fTimeDelta) override;



private:
	VTXPOINT_PARTICLE_INSTANCE* m_pVertexInstances = { nullptr };
	_float*						m_pSpeeds = { nullptr };
	/* 파티클이 생성되는 시작점? */
	_float3					    m_vPivot = {};
	_bool                       m_isLoop = { false };


public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const DESC* pArg);
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strParticleFilePath);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free() override;
};

END