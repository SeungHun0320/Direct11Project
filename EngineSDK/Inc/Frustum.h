#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void    Transform_ToWorldSpace();
	void    Transform_ToLocalSpace(_fmatrix WorldMatrix);

public:
	_bool   isIn_WorldSpace(_fvector vWorldPos, _float fRange);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	_float4			m_vOriginalPoints[8] = {};
	_float4			m_vWorldPoints[8] = {};
	_float4         m_vLocalPoints[8] = {};

	_float4			m_vWorldPlanes[6] = {};
	_float4         m_vLocalPlanes[6] = {};

private:
	void Make_Plane(const _float4* pPoints, _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void Free() override;
};

END