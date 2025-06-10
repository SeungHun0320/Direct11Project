#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)

class CButton final : public CUIObject
{
public:
	typedef struct tagButtonDesc : public CUIObject::DESC
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
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END