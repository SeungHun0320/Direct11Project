#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerSPBar final : public CUIContainerPart
{
public:
	enum PART {
		PART_BACK, PART_STAMINABAR, PART_STAMINABARCAP,
		PART_NOTCH_START, PART_NOTCH = 5,
		PART_END = PART_NOTCH
	};

public:
	typedef struct tagUI2DPlayerSPDesc : public CUIContainerPart::DESC
	{
		_float* pParentStamina{ nullptr };
		_float* pParentMaxStamina{ nullptr };
		_float* pParentStaminaRecorveryStat{ nullptr };
	}DESC;

private:
	CUI2D_PlayerSPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerSPBar(const CUI2D_PlayerSPBar& Prototype);
	virtual ~CUI2D_PlayerSPBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_UIVisible(_uint iPart, _bool isVisible) override;

private:
	_float* m_pParentStamina = { nullptr };
	_float* m_pParentMaxStamina = { nullptr };
	_float* m_pParentStaminaRecorveryStat = { nullptr };

private:
	_float m_fStaminaRatio = {};
	_float m_fLerpSpeed = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerSPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END