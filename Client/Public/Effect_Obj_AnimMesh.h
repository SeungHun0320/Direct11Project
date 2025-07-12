#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CEffect_Obj_AnimMesh : public CGameObject
{
public:
	typedef struct tagEffectPartAnimMeshDesc : public CGameObject::DESC
	{
		LEVEL eLevelID{ LEVEL::LEVEL_END };
		_wstring  strPrototypeModelTag;
	}DESC;

protected:
	CEffect_Obj_AnimMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Obj_AnimMesh(const CEffect_Obj_AnimMesh& Prototype);
	virtual ~CEffect_Obj_AnimMesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel*  m_pModelCom = { nullptr };

protected:
	virtual void Reset_Animation();
	virtual void Set_MeshVisible(_uint iMeshIndex, _bool IsVisible);

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END