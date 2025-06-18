#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Client)

class CEnvironment_Object abstract : public CContainerObject
{
public:
	typedef struct tagEnvironmentObjectDesc : public CContainerObject::DESC
	{
		LEVEL eLevelID;
	}DESC;

public:
	enum PART { PART_BODY, PART_INTERACTION, PART_EFFECT, PART_END };

protected:
	CEnvironment_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnvironment_Object(const CEnvironment_Object& Prototype);
	virtual ~CEnvironment_Object() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner);

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	_bool m_isCollision = { false };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END