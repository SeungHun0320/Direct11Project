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
	enum UI_PASS { PASS_DEFAULT, PASS_BLEND, PASS_BLEND_POINT, PASS_VERTICAL, PASS_HORIZONTAL_L2R, PASS_HORIZONTAL_R2L, PASS_END };

public:
	typedef struct tagUIDesc : public CPartObject::DESC
	{
		LEVEL*	 pParentLevelID{ nullptr };
		_float	 fX, fY, fSizeX, fSizeY;
		_float   fOffset{};
		_wstring strPrototypeTag;
		_uint    iTextureIndex;
		UI_PASS  eUIPass{ PASS_BLEND };
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
	virtual HRESULT Initialize_Prototype(UI_TYPE eType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_PositionY(_float fY, _float fYOffset = 0.f);
	_vector Get_State(STATE eState);
	_matrix Get_CombinedMatrix();
	void Set_State(STATE eState, _fvector vState);


protected:
	CShader*		   m_pShaderCom   = { nullptr };
	CTexture*		   m_pTextureCom  = { nullptr };
	CVIBuffer_Rect*    m_pVIBufferCom = { nullptr };

protected:
	LEVEL*		m_pLevelID = { nullptr };
	UI_TYPE		m_eType = {};
	UI_PASS	    m_eUIPass = { PASS_END };

protected:
	_bool  m_bVisible = { true };
	_uint  m_iTextureIndex = {};

protected:
	_float          m_fOffset{};
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT			Bind_ShaderResources();
	
public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free()override;
};

END