#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_Inventory final : public CUIContainerPart
{
public:
	enum PART { PART_MASK,
		/* æ∆¿Ã≈€ΩΩ∑‘µÈ */
		PART_PASSIVEITEMSLOT_START,
		PART_USEITEMSLOT_START, PART_USEITEMSLOT = 5,
		PART_WEAPONSLOT_START, PART_WEAPONSLOT_END = 8,
		/*µﬁ πË∞Ê*/
		PART_PASSIVE_DIVIDER, PART_USEITEM_DIVIDER, PART_WEAPON_DIVIDER,
		PART_MONEYTEXT, PART_PASSIVEITEMTEXT, PART_USEITEMTEXT, PART_WEAPONTEXT,
		PART_TAB, PART_LINE, PART_HEXAGON, PART_COINICON,
		PART_END };

	enum SLOT {
		SLOT_PASSIVEITEM,
		SLOT_USEITEM0, SLOT_USEITEM1, SLOT_USEITEM2, SLOT_USEITEM3,
		SLOT_WEAPON0, SLOT_WEAPON1, SLOT_WEAPON2, SLOT_END
	};

public:
	typedef struct tagUI2DInventoryDesc : public CUIContainerPart::DESC
	{
		_bool* pParentIsOnInven{ nullptr };
		class CInventory* pInventory{ nullptr };

	}DESC;

private:
	CUI2D_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_Inventory(const CUI2D_Inventory& Prototype);
	virtual ~CUI2D_Inventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CInventory* m_pInventory = { nullptr };

private:
	_bool* m_pParentIsOnInven = { nullptr };

private:
	_int m_iSelectSlotIndex{};

	_uint m_iNumInvenSlots{};
	vector<class CUI2D_InventorySlot*> m_InvenSlots;


private:
	void Key_Input();

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END