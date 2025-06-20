#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CInventory final : public CPartObject
{
public:
	typedef struct tagInventoryDesc : public CPartObject::DESC
	{
		LEVEL* pParentLevelID;
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
	void Acquire_Potion();


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

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END