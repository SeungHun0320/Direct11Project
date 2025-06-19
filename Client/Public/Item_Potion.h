#pragma once

#include "Item.h"

BEGIN(Client)

class CItem_Potion final : public CItem
{
public:
	typedef struct tagPotionDesc : public CItem::DESC
	{

	}DESC;
private:
	CItem_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Potion(const CItem_Potion& Prototype);
	virtual ~CItem_Potion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CItem_Potion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END