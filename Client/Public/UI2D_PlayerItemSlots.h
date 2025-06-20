#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerItemSlots final : public CUIContainerPart
{
public:
	enum PART { PART_J, PART_K, PART_L,
				PART_J_ITEMS, PART_K_ITEMS, PART_L_ITEMS, PART_END };

public:
	typedef struct tagUI2DPlayerItemSlotsDesc : public CUIContainerPart::DESC
	{
		class CInventory* pInventory{ nullptr };
	}DESC;

private:
	CUI2D_PlayerItemSlots(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerItemSlots(const CUI2D_PlayerItemSlots& Prototype);
	virtual ~CUI2D_PlayerItemSlots() = default;

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
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerItemSlots* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END