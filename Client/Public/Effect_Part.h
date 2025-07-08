#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)

class CEffect_Part : public CPartObject
{
public:
	enum PARTICLE_ORIENTATION { WORLD, LOCAL, ORIENTATION_END };

public:
	typedef struct tagParticlePartDesc : public CPartObject::DESC
	{
		_wstring strParticeFilePath{};

		_wstring    strParticleTextureTag{};
		_wstring    strParticleBufferTag{};

		LEVEL*               pParentLevelID = { nullptr };
		PARTICLE_ORIENTATION eOrientation = { WORLD };

	}DESC;

protected:
	CEffect_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Part(const CEffect_Part& Prototype);
	virtual ~CEffect_Part() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Reset_Effect();
	void Set_ParentMatrix(_matrix ParentMatrix) { XMStoreFloat4x4(&m_ParentMatrix, ParentMatrix); }

protected:
	CShader*  m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

protected:
	_float4		m_vColor = {};
	LEVEL*		m_pParentLevelID = { nullptr };
	_float4x4   m_ParentMatrix = {};

protected:
	EFFECT_PASS m_ePass = { EFFECT_PASS::PASS_END };
	EFFECT_MOVE m_eMoveType = { EFFECT_MOVE::MOVE_END };
	PARTICLE_ORIENTATION m_eOrientation = { PARTICLE_ORIENTATION::ORIENTATION_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Desc(const wstring& strParticleFilePath);
	HRESULT Bind_ShaderResources();

public:
	static CEffect_Part* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END