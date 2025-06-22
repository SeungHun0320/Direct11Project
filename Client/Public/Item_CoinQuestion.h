#pragma once

#include "Item.h"

class CItem_CoinQuestion final : public CItem
{
public:
	typedef struct tagItemCoinQuestionDesc : public CItem::DESC
	{

	}DESC;

private:
	CItem_CoinQuestion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_CoinQuestion(const CItem_CoinQuestion& Prototype);
	virtual ~CItem_CoinQuestion() = default;
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
	static CItem_CoinQuestion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

