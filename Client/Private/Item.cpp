#include "Item.h"

#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CItem::CItem(const CItem& Prototype)
	: CContainerObject(Prototype)
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CItem::Update(_float fTimeDelta)
{
	return 	__super::Update(fTimeDelta);
}

void CItem::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem::Render()
{
	return S_OK;
}

void CItem::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
}

HRESULT CItem::Ready_Components(void* pArg)
{
	return S_OK;
}


void CItem::Free()
{
	__super::Free();
}