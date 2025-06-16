#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerMPBar final : public CUIContainerPart
{
public:
	enum PART {
		PART_BACK, PART_MANABAR, PART_MANABARCAP,
		PART_NOTCH_START, PART_NOTCH = 5,
		PART_END = PART_NOTCH
	};

public:
	typedef struct tagUI2DPlayerMPBarDesc : public CUIContainerPart::DESC
	{
		_float* pParentMana{ nullptr };
		_float* pParentMaxMana{ nullptr };
		_float* pParentManaRecorveryStat{ nullptr };
	}DESC;

private:
	CUI2D_PlayerMPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerMPBar(const CUI2D_PlayerMPBar& Prototype);
	virtual ~CUI2D_PlayerMPBar() = default;

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
	_float* m_pParentMana = { nullptr };
	_float* m_pParentMaxMana = { nullptr };
	_float* m_pParentManaRecorveryStat = { nullptr };

private:
	_float m_fManaRatio = {};
	_float m_fLerpSpeed = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerMPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END