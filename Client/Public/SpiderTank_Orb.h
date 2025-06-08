#pragma once

#include "Monster_Bullet.h"

BEGIN(Client)

class CSpiderTank_Orb final : public CMonster_Bullet
{
public:
	typedef struct tagSpiderTankOrb : public CMonster_Bullet::DESC
	{
		_float3 vDir{};
	}DESC;

private:
	CSpiderTank_Orb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderTank_Orb(const CSpiderTank_Orb& Prototype);
	virtual ~CSpiderTank_Orb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float3 m_vVelocity = {};
	_float3 m_vDir = {};

	_bool  m_bGrounded = { false };

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CSpiderTank_Orb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END