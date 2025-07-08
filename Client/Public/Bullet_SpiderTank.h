#pragma once

#include "Bullet_Monster.h"

BEGIN(Client)

class CBullet_SpiderTank final : public CBullet_Monster
{
public:
	typedef struct tagSpiderBulletOrb : public CBullet_Monster::DESC
	{

	}DESC;

private:
	CBullet_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_SpiderTank(const CBullet_SpiderTank& Prototype);
	virtual ~CBullet_SpiderTank() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Create_Trail();

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBullet_SpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END