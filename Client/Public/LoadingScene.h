#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)

class CLoadingScene final : public CUIObject
{
public:
	typedef struct tagLoadingSceneDesc : public CUIObject::DESC
	{
		LEVEL eLevelID{};
	}DESC;

private:
	CLoadingScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingScene(const CLoadingScene& Prototype);
	virtual ~CLoadingScene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVEL  m_eLevelID = { LEVEL::LEVEL_END };
	_float m_fAlpha = {1};
private:
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CLoadingScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END