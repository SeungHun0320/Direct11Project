#pragma once

#include "Effect_Part.h"

BEGIN(Client)

class CEffect_BossSteam final : public CEffect_Part
{
public:
	enum TEX_TYPE { MASK, NOISE, TEX_END };
	enum MOVETYPE { SPREAD, CHASE, MOVE_END };

public:
	typedef struct tagEffectSteamDesc : public CEffect_Part::DESC
	{
		const _bool* pParentisInBattle{ nullptr };
	}DESC;

private:
	CEffect_BossSteam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_BossSteam(const CEffect_BossSteam& Prototype);
	virtual ~CEffect_BossSteam() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pEffectTextureCom[TEX_END] = { nullptr };

private:
	const _bool* m_pParentisInBattle = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static  CEffect_BossSteam* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END