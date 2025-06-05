#pragma once

#include "Client_Defines.h"
#include "Body_Environment_Object.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Chest final : public CBody_Environment_Object
{
public:
	typedef struct tagBodyChestDesc : public CBody_Environment_Object::DESC
	{

	}DESC;
private:
	CBody_Chest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Chest(const CBody_Chest& Prototype);
	virtual ~CBody_Chest() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID)override;


public:
	static CBody_Chest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END