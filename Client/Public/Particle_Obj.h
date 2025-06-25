#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CParticle_Obj : public CGameObject
{
public:
	typedef struct tagParticleObjDesc : public CGameObject::DESC
	{
		LEVEL eLevelID = { LEVEL::LEVEL_END };
		_wstring strParticleBufferTag{};
		_wstring strParticleTextureTag{};

	}DESC;

private:
	CParticle_Obj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Obj(const CParticle_Obj& Prototype);
	virtual ~CParticle_Obj() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	LEVEL   m_eLevelID = { LEVEL::LEVEL_END };

private:
	CShader*				  m_pShaderCom = { nullptr };
	CTexture*				  m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Obj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END