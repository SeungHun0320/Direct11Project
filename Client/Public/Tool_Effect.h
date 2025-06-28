#pragma once

#include "Tool.h"

BEGIN(Engine)
class CVIBuffer_Point_Instance_Tool;
END


BEGIN(Client)

class CParticle_Tool;

class CTool_Effect final : public CTool
{
public:
	typedef struct tagParticleToolDesc
	{
		_uint		iNumInstance{500};
		_float2		vSize{1.f, 2.f};
		_float3		vRange{5.f, 5.f, 5.f};
		_float3		vCenter{0.f, 0.f, 0.f};
		_float3		vPivot{0.f, 0.f, 0.f};
		_float2		vLifeTime{1.f, 2.f};
		_float2		vSpeed{1.f, 2.f};
		_bool		isLoop{true};
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

	CParticle_Tool*				   m_pParticleTool = { nullptr };
	CVIBuffer_Point_Instance_Tool* m_pVIBufferTool = { nullptr };

private:
	const _float m_fMinSize{ 0.01f },     m_fMaxSize{ 5.f };
	const _float m_fMinRange{ 0.f },    m_fMaxRange{ 1000.f };
	const _float m_fMinCenter{ -500.f },   m_fMaxCenter{ 500.f };
	const _float m_fMinPivot{ -500.f },    m_fMaxPivot{ 500.f };
	const _float m_fMinLifeTime{ 0.f }, m_fMaxLifeTime{ 10.f };
	const _float m_fMinSpeed{ 0.f },    m_fMaxSpeed{ 100.f };

private:
	void Adjust_ParticleDesc();
	void Change_TextureListBox();

private:
	HRESULT Ready_Tool_Particle(const _wstring& strLayerTag);

public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END