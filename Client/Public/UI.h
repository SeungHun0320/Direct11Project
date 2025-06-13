#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI abstract : public CPartObject
{
public:
	typedef struct tagUIDesc : public CPartObject::DESC
	{
		LEVEL	eLevelID;
		_float	fX, fY, fSizeX, fSizeY;
	}DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& Prototype);
	virtual ~CUI() = default;

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
	_bool  m_bVisible = { false };

protected:
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
	
public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free()override;
};

END