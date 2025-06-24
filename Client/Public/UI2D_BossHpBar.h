#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_BossHPBar final : public CUIContainerPart
{
public:
	enum PART {
		PART_BACK, PART_HPBAR, PART_DIVIDER_START, PART_DIVIDER = 4, PART_NAME, PART_END
	};

public:
	typedef struct tagUI2DPlayerHPBarDesc : public CUIContainerPart::DESC
	{
		_float* pParentHP{ nullptr };
		_float* pParentMaxHP{ nullptr };
		_bool*  pParentisInBattle{ nullptr };
	}DESC;

private:
	CUI2D_BossHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_BossHPBar(const CUI2D_BossHPBar& Prototype);
	virtual ~CUI2D_BossHPBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float* m_pParentHP = { nullptr };
	_float* m_pParentMaxHP = { nullptr };
	_bool*  m_pParentisInBattle{ nullptr };

private:
	_float m_fHpRatio = {};
	_float m_fLerpSpeed = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_BossHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END