#pragma once

#include "Bullet_Monster.h"
#include "PartObject.h"

BEGIN(Client)

class CBullet_SpiderTankOrb final : public CBullet_Monster
{
public:
	typedef struct tagSpiderTankOrb : public CBullet_Monster::DESC
	{

	}DESC;

private:
	CBullet_SpiderTankOrb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_SpiderTankOrb(const CBullet_SpiderTankOrb& Prototype);
	virtual ~CBullet_SpiderTankOrb() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float3 m_vVelocity = {};

	_bool  m_bGrounded = { false };
	_bool  m_isShoot = { true };

private:
	CPartObject* m_pPartObject = { nullptr };

private:
	virtual HRESULT Craete_SmokeEffect() override;
	HRESULT Create_Steam();

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);

public:
	static CBullet_SpiderTankOrb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END