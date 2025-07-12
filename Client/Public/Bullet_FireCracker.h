#pragma once

#include "Bullet.h"

class CBullet_FireCracker final : public CBullet
{
public:
	typedef struct tagFireCrackerDesc : public CBullet::DESC
	{

	}DESC;
private:
	CBullet_FireCracker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_FireCracker(const CBullet_FireCracker& Prototype);
	virtual ~CBullet_FireCracker() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float m_fExplosionTime = {};
	_bool  m_isExplosion = { false };

private:
	_float3 m_vVelocity = {};
	_float  m_fGravity = { -40.f };

private:
	CCollider* m_pExplosionCollider = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

private:
	void    Thrown(_float fTimeDelta);
	HRESULT Explosion();

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBullet_FireCracker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

