#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

class CBody_Environment_Object abstract : public CPartObject
{
public:
	typedef struct tagBodyEnvironmentObjectDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;

protected:
	CBody_Environment_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Environment_Object(const CBody_Environment_Object& Prototype);
	virtual ~CBody_Environment_Object() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader*   m_pShaderCom = { nullptr };
	CModel*    m_pModelCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free();
};

