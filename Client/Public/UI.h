#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI : public CPartObject
{
public:
	typedef struct tagUIDesc : public CPartObject::DESC
	{
		LEVEL	 eLevelID;
		_float	 fX, fY, fSizeX, fSizeY;
		_wstring strPrototypeTag;
	}DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& Prototype);
	virtual ~CUI() = default;

public:
	void Set_Visible(_bool isVisible){
		m_bVisible = isVisible;	};

	void Set_TextureIndex(_uint iTextureIdx) {
		m_iTextureIndex = iTextureIdx;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CShader*		   m_pShaderCom   = { nullptr };
	CTexture*		   m_pTextureCom  = { nullptr };
	CVIBuffer_Rect*    m_pVIBufferCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	_bool  m_bVisible = { true };
	_uint  m_iTextureIndex = {};

protected:
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	
public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free()override;
};

END