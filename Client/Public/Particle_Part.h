#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CParticle_Part : public CPartObject
{
public:
	typedef struct tagParticlePartDesc : public CPartObject::DESC
	{
		_wstring strParticleBufferTag{};
		_wstring strParticleTextureTag{};
		const _float4x4* pSocketMatrix = { nullptr };
		LEVEL* pParentLevelID = { nullptr };

	}DESC;

private:
	CParticle_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Part(const CParticle_Part& Prototype);
	virtual ~CParticle_Part() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

private:
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	LEVEL*  m_pParentLevelID = { nullptr };



private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CParticle_Part* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END