#pragma once

#include "Monster_Bullet.h"

class CSpiderTank_Lager final : public CMonster_Bullet
{
public:
	typedef struct tagSpiderBulletOrb : public CMonster_Bullet::DESC
	{
		const _float4x4* pParentMatrix{ nullptr };
	}DESC;

private:
	CSpiderTank_Lager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderTank_Lager(const CSpiderTank_Lager& Prototype);
	virtual ~CSpiderTank_Lager() = default;

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
	static CSpiderTank_Lager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

