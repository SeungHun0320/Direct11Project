#pragma once

#include "Boss.h"

BEGIN(Client)

class CSpiderTank final : public CBoss
{
public:
	typedef struct tagSpiderTank : public CBoss::DESC
	{

	}DESC;

private:
	CSpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderTank(const CSpiderTank& Prototype);
	virtual ~CSpiderTank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_uint m_iSoonseo = {};

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CSpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
