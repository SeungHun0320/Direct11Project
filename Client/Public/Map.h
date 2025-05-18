#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CMap abstract : public CGameObject
{
public:
	typedef struct tagMapDesc : public CGameObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
protected:
	CMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMap(const CMap& Prototype);
	virtual ~CMap() = default;

public:
	_float3 Get_PickedPos_World();
	_float3 Get_PickedPos_World_Snap();
	_float3 Get_PickedPos_Local();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free()override;
};

END