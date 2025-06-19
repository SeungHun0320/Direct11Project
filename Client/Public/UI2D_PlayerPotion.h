#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_PlayerPotion final : public CUIContainerPart
{
public:
	enum PART { PART_POTION = 3, PART_PKEY, PART_END };

public:
	typedef struct tagUI2DPlayerPotionDesc : public CUIContainerPart::DESC
	{
		_int* pParentNumPotion{ nullptr };
		_int* pParentCurPotion{ nullptr };
	}DESC;

private:
	CUI2D_PlayerPotion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerPotion(const CUI2D_PlayerPotion& Prototype);
	virtual ~CUI2D_PlayerPotion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_int* m_pParentNumPotion = { nullptr };
	_int* m_pParentCurPotion = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_PlayerPotion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END