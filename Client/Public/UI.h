#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI : public CUIObject
{
public:
	typedef struct tagUIDesc : public CUIObject::DESC
	{
		LEVEL eLevelID{};
	}DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& Prototype);
	~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	/* 어떤 레벨에서 쓸지 오브젝트 별로 변수 필요함 */

protected:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END