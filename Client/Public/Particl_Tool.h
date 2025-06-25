#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance_Tool;
END

BEGIN(Client)

class CParticl_Tool final : public CGameObject
{
public:
	typedef struct tagParticleObjDesc : public CGameObject::DESC
	{
		_wstring strParticleTextureTag{};
	}DESC;
	
private:
	CParticl_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticl_Tool(const CParticl_Tool& Prototype);
	virtual ~CParticl_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance_Tool*  m_pVIBufferCom = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CParticl_Tool* Craete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END