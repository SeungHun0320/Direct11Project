#pragma once

#include "Monster_Bullet.h"

BEGIN(Client)

class CSpiderTank_Bullet final : public CMonster_Bullet
{
public:
	typedef struct tagSpiderBulletOrb : public CMonster_Bullet::DESC
	{

	}DESC;

private:
	CSpiderTank_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderTank_Bullet(const CSpiderTank_Bullet& Prototype);
	virtual ~CSpiderTank_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CSpiderTank_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END