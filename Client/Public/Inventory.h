#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CInventory final : public CPartObject
{
public:
	typedef struct tagInventoryDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	};

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

public:
	_int Get_Coin() const {
		return m_iCoin;
	}
	void Add_Coin(_int iPrice) {
		m_iCoin += iPrice;
	};


private:
	_int m_iCoin = {};

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END