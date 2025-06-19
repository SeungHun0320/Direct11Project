#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerHPBar final : public CUIContainerPart
{
public:
	enum PART { PART_BACK, PART_HPBAR, PART_HPBARCAP,
		PART_NOTCH_START, PART_NOTCH = 5,
		PART_END = PART_NOTCH};

public:
	typedef struct tagUI2DPlayerHPBarDesc : public CUIContainerPart::DESC
	{
		_float* pParentHP{ nullptr };
		_float* pParentMaxHP{ nullptr };
		_float* pParentHPRecorveryStat{ nullptr };
	}DESC;

private:
	CUI2D_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerHPBar(const CUI2D_PlayerHPBar& Prototype);
	virtual ~CUI2D_PlayerHPBar() = default;

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
	_float* m_pParentHPRecorveryStat = { nullptr };

private:
	_float m_fHpRatio = {};
	_float m_fLerpSpeed = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END