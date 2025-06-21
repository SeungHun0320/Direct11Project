#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Client)

class CItem : public CContainerObject
{
public:
	enum PART {PART_BODY, PART_INTERACTION, PART_PRICE, PART_UISHOP, PART_REWARD, PART_END};

public:
	typedef struct tagItemDesc : public CContainerObject::DESC
	{
		LEVEL eLevelID;
		ITEM_TYPE eItemType{ ITEM_TYPE::IT_END };

	}DESC;
protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& Prototype);
	virtual ~CItem() = default;

public: /* ¸ÊÅø ½È¾î */
	ITEM_TYPE Get_ItemType() const {
		return m_eItemType;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	ITEM_TYPE m_eItemType = { ITEM_TYPE::IT_END };

protected:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END