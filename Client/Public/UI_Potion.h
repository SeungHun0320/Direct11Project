#pragma once

#include "UI.h"

class CUI_Potion final : public CUI
{
public:
	typedef struct tagUIPotionDessc :public CUI::DESC
	{

	}DESC;

private:
	CUI_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Potion(const CUI_Potion& Prototype);
	virtual ~CUI_Potion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CUI_Potion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

