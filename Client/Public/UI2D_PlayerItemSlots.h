#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerItemSlots final : public CUIContainerPart
{
public:
	enum PART { PART_J, PART_K, PART_L, PART_END };

public:
	typedef struct tagUI2DPlayerItemSlotsDesc : public CUIContainerPart::DESC
	{

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

public:
	virtual void Set_UIVisible(_uint iPart, _bool isVisible) override;
	virtual void Set_TextureIndex(_uint iPart, _uint iTextureIdx)override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerItemSlots* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END