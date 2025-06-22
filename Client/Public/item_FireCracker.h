#pragma once

#include "Item.h"

class CItem_FireCracker final : public CItem
{
public:
	enum TYPE { SHOP, THROWABLE, TYPE_END };

public:
	typedef struct tagItemFireCrackerDesc : public CItem::DESC
	{
	}DESC;

private:
	CItem_FireCracker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_FireCracker(const CItem_FireCracker& Prototype);
	virtual ~CItem_FireCracker() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CItem_FireCracker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

