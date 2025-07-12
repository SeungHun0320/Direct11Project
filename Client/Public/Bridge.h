#pragma once

#include"Environment_Object.h"

BEGIN(Client)

class CBridge final : public CEnvironment_Object
{
public:
	typedef struct tagBridgeDesc : public CEnvironment_Object::DESC
	{

	}DSEC;

private:
	CBridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBridge(const CBridge& Prototype);
	virtual ~CBridge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() override;

public:
	static CBridge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;

};

END