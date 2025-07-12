#pragma once

#include "Part_Player.h"

BEGIN(Engine)
class CVIBuffer_Trail;
class CTexture;
END

BEGIN(Client)

class CWeapon_Player final : public CPart_Player
{
public:
	enum SOCKET { SWORD, SWORD_TRAIL, SOCKET_END };

public:
	typedef struct tagPlayerWeaponDesc : public CPart_Player::DESC
	{
		const _float4x4*   pSocketMatrix[SOCKET_END]{ nullptr };
		const WEAPON_TYPE* pParentWeaponType{ nullptr };
		const _bool*	   pParentisAttacked{ nullptr };
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
	CCollider*			m_pColliders[ENUM_CLASS(WEAPON_TYPE::WT_END)] = { nullptr };
	CVIBuffer_Trail*	m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };

private:
	const _float4x4*	m_pSocketMatrix[SOCKET_END] = {nullptr};
	const WEAPON_TYPE*	m_pParentWeaponType = { nullptr };
	const _bool*		m_pParentisAttacked = { nullptr };
private:
	_float3 m_vColorStart{}, m_vColorEnd{};
	_float  m_fPowerAlpha{}, m_fPowerColor{};
	_float  m_fAlpha{};

private:
	void Update_Trail(const _float4x4* pBoneMatrix, const _float4x4* pTrailMatrix);

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CWeapon_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END