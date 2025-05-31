#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

class CBody_SpiderTank final : public CPartObject
{
public:
	typedef struct tagBodySpiderTankDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
private:
	CBody_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_SpiderTank(const CBody_SpiderTank& Prototype);
	virtual ~CBody_SpiderTank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_SpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

