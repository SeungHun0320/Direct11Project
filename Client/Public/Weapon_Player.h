#pragma once

#include "Part_Player.h"

BEGIN(Client)

class CWeapon_Player final : public CPart_Player
{
public:
	typedef struct tagPlayerWeaponDesc : public CPart_Player::DESC
	{

	}DESC;

private:
	CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player(const CWeapon_Player& Prototype);
	virtual ~CWeapon_Player() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Active(WEAPON_TYPE eType, _bool isActive);

private:
	CCollider* m_pColliders[ENUM_CLASS(WEAPON_TYPE::WT_END)] = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CWeapon_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END