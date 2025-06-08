#pragma once

#include "Bullet.h"

BEGIN(Client)

class CMonster_Bullet abstract : public CBullet
{
public:
	typedef struct tagMonsterBulletDesc : public CBullet::DESC
	{
		_float3 vDir{};
	}DESC;

protected:
	CMonster_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Bullet(const CMonster_Bullet& Prototype);
	virtual ~CMonster_Bullet() = default;

public:
	_float Get_AttackValue() const {
		return m_fAttack;
	}
	_float Get_StaggerValue() const {
		return m_fStaggerValue;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected: /* 맵툴에선 굳이 넣어줄 필요가 없어서 그냥 이니셜라이즈에서 때려박는게 나은거 같기도 */
	CTransform* m_pTargetTransform = { nullptr };

protected:
	_float3 m_vDir{};

protected:
	_float m_fTimeAcc = {};
	_float m_fDeadTime = {};

protected: /* 공격력 */
	_float m_fAttack = {};
	_float m_fStaggerValue = {};

protected:
	virtual HRESULT Ready_Components(void* pArg);

public:
	CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END