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
		_float2		vSize{0.5f, 1.f};
		_float3		vRange{5.f, 5.f, 5.f};
		_float3		vCenter{0.f, 0.f, 0.f};
		_float3		vPivot{0.f, 0.f, 0.f};
		_float2		vLifeTime{1.f, 2.f};
		_float2		vSpeed{1.f, 2.f};
		_float2     vFrameXY{};
		_float2     vFrameSpeed{};
		_float2     vRotationZ{ 0.f, XM_2PI };
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

public:
	void Save_Load_Menu();

private: /* ÆÄÆ¼Å¬ Desc */
	DESC m_ParticleDesc = {};

	CParticle_Tool*				   m_pParticleTool = { nullptr };
	CVIBuffer_Point_Instance_Tool* m_pVIBufferTool = { nullptr };

private:
	const _float m_fMinSize{ 0.01f };
	const _float m_fMinRange{ 0.5f };
	const _float m_fMinCenter{ 0.1f };
	const _float m_fMinPivot{ 0.1f };
	const _float m_fMinLifeTime{ 0.05f };
	const _float m_fMinSpeed{ 0.1f };

private:
	EFFECT_PASS m_ePass = { EFFECT_PASS::PASS_END };
	EFFECT_MOVE m_eMove = { EFFECT_MOVE::MOVE_END };

private:
	_float4 m_vColor{1.f, 1.f, 1.f, 1.f};
	_bool   m_isSprite = { false };
	_bool   m_isSoft = { false };

private:
	vector<_string> m_ProtoTextureNames;

private:
	_string m_strParticleTag = {};

private:
	void Add_ListBoxName();

private:
	void Adjust_ParticleDesc();
	void Adjust_ParticeSpriteDesc();
	void Change_TextureListBox();

private:
	void Save_Menu();
	void Load_Menu();
	HRESULT Save_ParticleDesc(const _string& strParticleTag);
	HRESULT Load_ParticleDesc(const _string& strParticleTag);

private:
	HRESULT Ready_Tool_Particle(const _wstring& strLayerTag);

public:
	static CTool_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END