#pragma once

#include "UI2DContainerPart.h"

class CUI2D_PlayerHPBar final : public CUI2DContainerPart
{
public:
	enum PART { PART_HP, PART_END };

public:
	typedef struct tagUI2DPlayerHPBarDesc : public CUI2DContainerPart::DESC
	{

	}DESC;

private:
	CUI2D_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_PlayerHPBar(const CUI2D_PlayerHPBar& Prototype);
	virtual ~CUI2D_PlayerHPBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;


public:
	static CUI2D_PlayerHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

