#pragma once
#include "Environment_Object.h"

BEGIN(Client)

class CChest final : public CEnvironment_Object
{
public:
	typedef struct tagChestDesc : public CEnvironment_Object::DESC
	{

	}DESC;
private:
	CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChest(const CChest& Prototype);
	virtual ~CChest() = default;

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
	static CChest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END