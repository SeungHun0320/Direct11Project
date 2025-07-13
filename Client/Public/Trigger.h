#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CTrigger final : public CGameObject
{
public:

public:
	typedef struct tagTriggerDesc : public CGameObject::DESC
	{
		LEVEL		eLevelID{ LEVEL::LEVEL_END };
		COLLIDER_ID eColliderID{ COLLIDER_ID::CI_END };
	}DESC;

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& Prototype);
	virtual ~CTrigger() = default;

public:
	const COLLIDER_ID Get_Collider_ID() const {
		return m_eColliderID;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };

private:
	LEVEL			m_eLevelID = { LEVEL::LEVEL_END };
	COLLIDER_ID		m_eColliderID = { COLLIDER_ID::CI_END };

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner);

public:
	static CTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END