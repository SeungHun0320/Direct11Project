#pragma once

#include "Tool.h"

BEGIN(Client)

class CTool_Effect final : public CTool
{
public:
	typedef struct tagParticleToolDesc
	{
		_uint		iNumInstance{};
		_float2		vSize{};
		_float3		vRange{};
		_float3		vCenter{};
		_float3		vPivot{};
		_float2		vLifeTime{};
		_float2		vSpeed{};
		_bool		isLoop{};
	}DESC;
private:
	CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CTool_Effect() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ExtraUI() override;

private: /* ÆÄÆ¼Å¬ Desc */
	DESC m_ParticleDesc = {};


public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END