#pragma once

#include "Bullet.h"

BEGIN(Client)

class CBullet_Monster abstract : public CBullet
{
public:
	typedef struct tagMonsterBulletDesc : public CBullet::DESC
	{

	}DESC;

protected:
	CBullet_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet_Monster(const CBullet_Monster& Prototype);
	virtual ~CBullet_Monster() = default;

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
	virtual HRESULT Ready_Components(void* pArg);

public:
	CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END