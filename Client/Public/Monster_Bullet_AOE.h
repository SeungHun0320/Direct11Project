#pragma once

#include "Monster_Bullet.h"

class CMonster_Bullet_AOE abstract : public CMonster_Bullet
{
public:
	typedef struct tagMonsterBulletAOEDesc : public CMonster_Bullet::DESC
	{

	}DESC;
protected:
	CMonster_Bullet_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Bullet_AOE(const CMonster_Bullet_AOE& Prototype);
	virtual ~CMonster_Bullet_AOE() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

