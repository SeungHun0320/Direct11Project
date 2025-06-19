#pragma once

#include "UIContainerPart.h"

class CUI2D_Reward final : public CUIContainerPart
{
public:
	enum PART { PART_BACKING, PART_ITEMS, PART_END };

public:
	typedef struct tagUI2DRewardDesc : public CUIContainerPart::DESC
	{
		_bool*		pParentIsOpen{ nullptr };
		ITEM_TYPE*  pParentItemType{ nullptr };
	}DESC;

private:
	CUI2D_Reward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_Reward(const CUI2D_Reward& Prototype);
	virtual ~CUI2D_Reward() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	ITEM_TYPE*  m_pParentItemType = { nullptr };
	_bool*		m_pParentIsOpen = { nullptr };
	_float		m_fTimeAcc = {};
	_bool		m_isExpired = { false };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_Reward* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

