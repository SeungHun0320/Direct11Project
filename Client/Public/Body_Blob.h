#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Blob final : public CPartObject
{
public:
	typedef struct tagBodyBlobDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
private:
	CBody_Blob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Blob(const CBody_Blob& Prototype);
	virtual ~CBody_Blob() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool Play_Animation(_float fTimeDelta);
	 void Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	_int m_iSoonseo = {};

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_Blob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END