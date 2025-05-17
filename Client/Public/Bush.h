#pragma once

#include "Environment_Object.h"

BEGIN(Client)

class CBush final : public CEnvironment_Object
{
public:
	typedef struct tagBushDesc : public CEnvironment_Object::DESC
	{

	}DESC;
private:
	CBush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBush(const CBush& Prototype);
	virtual ~CBush() = default;

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
	static CBush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END