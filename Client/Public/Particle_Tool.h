#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance_Tool;
END

BEGIN(Client)

class CParticle_Tool final : public CGameObject
{
public:
	enum MOVEMENT { DROP, SPREAD, MOVEMENT_END };
	enum PASS { DEFAULT, TOOL, SPRITE, ROTATION,
				TOOL_BLEND, SPRITE_BLEND, ROTATION_BLEND, PASS_END };
public:
	enum TEX_TYPE { TEXTURE, MASK, NOISE, TEX_END };

public:
	typedef struct tagParticleObjDesc : public CGameObject::DESC
	{
		_wstring strParticleTextureTag{};
	}DESC;
	
private:
	CParticle_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Tool(const CParticle_Tool& Prototype);
	virtual ~CParticle_Tool() = default;

public:
	void Change_Color(_float4 vColor) {
		m_vColor = vColor;
	};

	void Change_Move(EFFECT_MOVE eType) {
		m_eType = eType;
	}

	void Change_Pass(EFFECT_PASS ePass) {
		m_ePass = ePass;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Change_TextureCom(const _wstring& strTextureTag);

private:
	_float4							m_vColor{1.f, 1.f, 1.f, 255.f};
	EFFECT_MOVE						m_eType{ EFFECT_MOVE::MOVE_END };
	EFFECT_PASS						m_ePass{ EFFECT_PASS::PASS_END };

	CTexture*						m_pTextureCom[TEX_END] = { nullptr };
	CShader*						m_pShaderCom = { nullptr };
	CVIBuffer_Point_Instance_Tool*  m_pVIBufferCom = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static  CParticle_Tool* Craete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END