#pragma once
#include "Client_Defines.h"
#include "Body_Environment_Object.h"

BEGIN(Client)

class CBody_Bridge final : public CBody_Environment_Object
{
public:
	typedef struct tagBodyBridgeDesc : public CBody_Environment_Object::DESC
	{

	}DESC;
private:
	CBody_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Bridge(const CBody_Bridge& Prototype);
	virtual ~CBody_Bridge() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg)override;

public:
	static CBody_Bridge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END