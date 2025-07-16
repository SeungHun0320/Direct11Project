#pragma once

#include "Bullet_Monster.h"
#include "PartObject.h"

BEGIN(Client)

class CBullet_SpiderTank final : public CBullet_Monster
{
public:
	typedef struct tagSpiderBulletDesc : public CBullet_Monster::DESC
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
	CNavigation* m_pNavigationCom = { nullptr };
	CPartObject* m_pPartObject = { nullptr };

private:
	HRESULT Create_Trail();
	HRESULT Create_Effect();

private:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);

public:
	static CBullet_SpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END