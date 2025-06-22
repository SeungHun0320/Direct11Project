#pragma once

#include "Bullet_Monster.h"

class CBullet_SpiderTankLager final : public CBullet_Monster
{
public:
	typedef struct tagSpiderBulletOrb : public CBullet_Monster::DESC
	{
		const _float4x4* pParentMatrix{ nullptr };
	}DESC;

private:
	CBullet_SpiderTankLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_SpiderTankLager(const CBullet_SpiderTankLager& Prototype);
	virtual ~CBullet_SpiderTankLager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	const _float4x4* m_pParentMatrix = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBullet_SpiderTankLager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

