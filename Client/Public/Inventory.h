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
	void Acquire_Potion();
	void Acquire_Shield(_bool isShield);


public: /* 코인 */
	_int Get_Coin() const { return m_iCoin; }
	void Add_Coin(_int iPrice) { m_iCoin += iPrice;	}

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

public: /* 무기들 */

public: /* 사용아이템들 */

public: /* 아이템 선택관련 */
	_int Get_SeletSlotIndex() const { return m_iSelectSlotIndex; }


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

private: /* 무기 */
	_bool m_bHasWeapon[3] = {};
	ITEM_TYPE m_eWeaponSlot[3] = {};
private: /* 사용 아이템 */
	_bool m_bHasUseItem[3] = {};
	ITEM_TYPE m_eUseItemSlot[3] = {};

private: /* 아이템 선택관련 */
	_int m_iSelectSlotIndex = {1};

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