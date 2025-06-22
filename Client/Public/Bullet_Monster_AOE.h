#pragma once

#include "Bullet_Monster.h"

class CBullet_Monster_AOE abstract : public CBullet_Monster
{
public:
	typedef struct tagMonsterBulletAOEDesc : public CBullet_Monster::DESC
	{

	}DESC;
protected:
	CBullet_Monster_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_Monster_AOE(const CBullet_Monster_AOE& Prototype);
	virtual ~CBullet_Monster_AOE() = default;

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

