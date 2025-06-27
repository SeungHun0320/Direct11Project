#pragma once

#include "UI_Sprite.h"

class CUI3D_PlayerSweat final : public CUI_Sprite
{
public:
	typedef struct tagUI3DPlayerSweatDesc : public CUI_Sprite::DESC
	{
		const _bool* pParentisNoStamina{ nullptr };
	}DESC;

private:
	CUI3D_PlayerSweat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI3D_PlayerSweat(const CUI3D_PlayerSweat& Prototype);
	virtual ~CUI3D_PlayerSweat() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	const _bool* m_pParentisNoStamina = { nullptr };

public:
	static CUI3D_PlayerSweat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free()override;
};

