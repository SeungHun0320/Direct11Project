#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CInventory final : public CPartObject
{
public:
	enum SLOT {
		SLOT_PASSIVEITEM = 0,
		SLOT_USEITEM0, SLOT_USEITEM1, SLOT_USEITEM2, SLOT_USEITEM3,
		SLOT_WEAPON0, SLOT_WEAPON1, SLOT_WEAPON2, SLOT_END
	};

	enum QSLOT_TYPE { QSLOT_J, QSLOT_K, QSLOT_L, QSLOT_END };

	typedef struct tagInventorySlot
	{
		ITEM_TYPE eType{ ITEM_TYPE::IT_END };
		_bool	  bHasItem{ false };
		_int	  iCount{};
	}INVEN_SLOT;

	typedef struct tagQuickSlot
	{
		ITEM_TYPE eType{ ITEM_TYPE::IT_END };
		_int iInvenSlotIndex{};
		_bool bHasItem{ false };
	}QUICK_SLOT;

public:
	typedef struct tagInventoryDesc : public CPartObject::DESC
	{
		LEVEL* pParentLevelID;
		_bool* pParentIsOnInven{ nullptr };
	}DESC;

private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& Prototype);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: /* 이벤트 매니저 용 */
	void Subscribe_Events();
	void Acquire_Item(ITEM_TYPE eType);

public: /* 코인 */
	_int Get_Coin() const { return m_iCoin; }
	void Add_Coin(_int iPrice) { m_iCoin += iPrice;	}
	void Use_Coin(_int iPrice) { m_iCoin -= iPrice; }
	void Buy_ShopItem(ITEM_TYPE eType, _int iPrice);

public: /* 포션 */
	_int Get_CurPotion() const { return m_iCurNumPotion; } /* 사용가능한 */
	_int Get_MaxPotion() const { return m_iMaxNumPotion; } /* 그냥 제한 */
	_int Get_NumPotion() const { return m_iNumPotion; }    /* 갖고있는 포션 */
	
	void Set_CurPotion(_int iNum) { m_iCurNumPotion = clamp(iNum, 0, m_iMaxNumPotion); }
	void Set_NumPotion(_int iNum) { m_iNumPotion = clamp(iNum, 0, m_iCurNumPotion); }

	void  Refill_Potion() { m_iCurNumPotion = m_iNumPotion; }
	_bool Add_Potion(_int iCount = 1);
	_bool Use_Potion();

public: /* 방패 */
	_bool Get_isShield() const { return m_isShield; }
	void  Set_isShield(_bool isShield) { m_isShield = isShield; }

public: /* 무기, 사용아이템 */
	INVEN_SLOT Get_Slot(SLOT eSlotIndex) const { return m_InvenSlot[eSlotIndex]; }

	void Add_UseItemSlot(ITEM_TYPE eType, _int iCount = 1);
	void Add_WeaponSlot(ITEM_TYPE eType);

	_bool Is_UseItem(ITEM_TYPE eType);
	_bool Use_QuickSlot(QSLOT_TYPE eSlot);

public: /* 아이템 선택관련 */
	_int Get_SeletSlotIndex() const { return m_iSelectSlotIndex; }

public: /* 퀵슬롯 관련 */
	QUICK_SLOT Get_QuickSlot(QSLOT_TYPE eQuickSlot) const { return m_QuickSlot[eQuickSlot]; }
	ITEM_TYPE IsWeaponInQuickSlot(QSLOT_TYPE eQuickSlot);

	void Register_QuickSlot(_uint iQuickSlotIndex);
	
private:
	LEVEL* m_pLevelID = { nullptr };

private: /* 코인 */
	_int m_iCoin = {};
	
private: /* 포션 */
	_int   m_iMaxNumPotion = { 4 };
	_int   m_iNumPotion = {};
	_int   m_iCurNumPotion = {};

private: /* 방패 소유중? */
	_bool  m_isShield = { false };

private: /* 무기, 사용아이템 */
	INVEN_SLOT m_InvenSlot[SLOT_END] = {};

private: /* 아이템 선택관련 */
	_int m_iSelectSlotIndex = {1};

private: /* 퀵슬롯 관련 */
	QUICK_SLOT m_QuickSlot[QSLOT_END] = {};

private:
	_bool* m_pParentIsOnInven = { nullptr };

private: /* 키입력 */
	void Key_Input();
	void Move_Selector(_uint iSlotIndex);

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END