#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CEffect_AnimMesh abstract : public CPartObject
{
public:
	typedef struct tagEffectAnimMeshDesc : public CPartObject::DESC
	{
		LEVEL*    pParentLevelID;
		_wstring  strPrototypeModelTag;
	}DESC;

protected:
	CEffect_AnimMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_AnimMesh(const CEffect_AnimMesh& Prototype);
	virtual ~CEffect_AnimMesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Reset_Animation();
	void Set_MeshVisible(_bool IsVisible);
	void Set_ParentMatrix(_matrix ParentMatrix) { XMStoreFloat4x4(&m_ParentMatrix, ParentMatrix); }

protected:
	LEVEL*   m_pParentLevelID = { nullptr };
	_float   m_fRatio = {};

protected:
	CShader*  m_pShaderCom = { nullptr };
	CModel*   m_pModelCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };

protected:
	_float4x4 m_ParentMatrix = {};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END