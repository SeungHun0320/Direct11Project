#pragma once

#include "Client_Defines.h"
#include "Body_Environment_Object.h"

BEGIN(Engine)
class CModel_Instance;
END


BEGIN(Client)

class CBody_Grass final : public CBody_Environment_Object
{
public:
	typedef struct tagBodyGrassDesc : public CBody_Environment_Object::DESC
	{

	}DESC;
private:
	CBody_Grass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Grass(const CBody_Grass& Prototype);
	virtual ~CBody_Grass() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBody_Grass* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END