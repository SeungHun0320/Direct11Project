#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_InventorySlot final : public CUIContainerPart
{
public:
	enum PART { PART_BACK, PART_ITEMS, PART_SELECTOR, PART_END };

public:
	typedef struct tagUI2DInventoryDesc : public CUIContainerPart::DESC
	{
		_bool* pParentIsOnInven{ nullptr };

	}DESC;

private:
	CUI2D_InventorySlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_InventorySlot(const CUI2D_InventorySlot& Prototype);
	virtual ~CUI2D_InventorySlot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_fvector Get_State(STATE eState);
	void Set_State(STATE eState, _fvector vState);
	void Set_Selected(_bool isSelected);

private:
	_bool* m_pParentIsOnInven = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_InventorySlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END