#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CGlobalShadow final : public CGameObject
{
public:
	typedef struct tagGlobalShadowDesc : public CGameObject::DESC
	{
		LEVEL eLevelID{ LEVEL::LEVEL_END };
	}DESC;

private:
	CGlobalShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGlobalShadow(const CGlobalShadow& Prototype);
	virtual ~CGlobalShadow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual LIFE	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render_Shadow() override;

private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };

private:
	_float4x4	m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};

	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components();

public:
	static CGlobalShadow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END