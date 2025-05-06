#pragma once
#include "UI.h"

class CButton : public CUI
{
public:
	typedef struct tagButtonDesc : public CUI::DESC
	{
		_float3 vScale{};

	}DESC;

private:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

