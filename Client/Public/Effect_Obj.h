#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CEffect_Obj : public CGameObject
{
public:
	typedef struct tagParticleObjDesc : public CGameObject::DESC
	{
		_wstring strParticeFilePath{};

		LEVEL eLevelID = { LEVEL::LEVEL_END };

		_wstring    strParticleTextureTag{};
		_wstring    strParticleBufferTag{};
		
	}DESC;

private:
	CEffect_Obj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Obj(const CEffect_Obj& Prototype);
	virtual ~CEffect_Obj() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	LEVEL   m_eLevelID = { LEVEL::LEVEL_END };

protected:
	_float4		m_vColor = {};

protected:
	EFFECT_PASS m_ePass = { EFFECT_PASS::PASS_END };
	EFFECT_MOVE m_eMoveType = { EFFECT_MOVE::MOVE_END };

protected:
	CShader*				  m_pShaderCom = { nullptr };
	CTexture*				  m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Desc(const wstring& strParticleFilePath);
	HRESULT Bind_ShaderResources();

public:
	static CEffect_Obj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END