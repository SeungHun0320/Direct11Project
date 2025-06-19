#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI3D_MobHPBar final : public CUIContainerPart
{
public:
	enum PART {
		PART_BACK, PART_HPBAR, PART_END
	};

public:
	typedef struct tagUI2DPlayerHPBarDesc : public CUIContainerPart::DESC
	{
		_float* pParentHP{ nullptr };
		_float* pParentMaxHP{ nullptr };
		_bool*  pParentIsTargeted{ nullptr };
	}DESC;

private:
	CUI3D_MobHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI3D_MobHPBar(const CUI3D_MobHPBar& Prototype);
	virtual ~CUI3D_MobHPBar() = default;

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
	_bool*  m_pParentIsTargeted = { nullptr };

private:
	_float m_fHpRatio = {};
	_float m_fLerpSpeed = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI3D_MobHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END