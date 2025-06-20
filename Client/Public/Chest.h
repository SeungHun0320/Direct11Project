#pragma once
#include "Environment_Object.h"

BEGIN(Client)

class CChest final : public CEnvironment_Object
{
public:
	typedef struct tagChestDesc : public CEnvironment_Object::DESC
	{
		ITEM_TYPE eType{ ITEM_TYPE::IT_END };
	}DESC;
private:
	CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChest(const CChest& Prototype);
	virtual ~CChest() = default;

public:
	ITEM_TYPE Get_ItemType() const {
		return m_eType;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* Ãæ µ¹ */
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

private:
	ITEM_TYPE m_eType = { ITEM_TYPE::IT_END };
	_bool*    m_pBodyIsOpened = { nullptr };
	_bool     m_IsOpend = { false };
	_float    m_fTimeAcc = {};

private:
	void On_Opened();
	HRESULT Spawn_Coin(const _float3& vSpawnPos);
	void Give_Item();

private:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() override;

public:
	static CChest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END