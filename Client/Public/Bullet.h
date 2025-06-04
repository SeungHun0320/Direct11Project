#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)

class CBullet abstract : public CGameObject
{
public:
	typedef struct tagBulletDesc : public CGameObject::DESC
	{
		LEVEL eLevelID = { LEVEL::LEVEL_END };
	}DESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Bind_ShaderResources();

public:
	CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END