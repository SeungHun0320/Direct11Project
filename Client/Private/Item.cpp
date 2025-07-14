#include "Item.h"

#include "GameInstance.h"
#include "UI3D_Interaction.h"
#include "UI2D_Purchase.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CItem::CItem(const CItem& Prototype)
	: CContainerObject(Prototype)
{
}

void CItem::Open_DealWindow(_bool isDeal)
{
	if (!m_isDeal && m_isCollision)
		m_isDeal = isDeal;
}

void CItem::Close_DealWindow(_bool isDeal)
{
	if (m_isDeal && m_isCollision)
		m_isDeal = isDeal;
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID  = pDesc->eLevelID;
	m_eItemType = pDesc->eItemType;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	Subscribe_Events();

	m_pTransformCom->Set_RotationPerSec(1.f);

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CItem::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);

	return 	__super::Update(fTimeDelta);
}

void CItem::Late_Update(_float fTimeDelta)
{
	if (!m_pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(STATE::POSITION), 3.f))
		return;

	__super::Late_Update(fTimeDelta);

	m_isCollision = false;
}

HRESULT CItem::Render()
{
	return S_OK;
}

void CItem::Subscribe_Events()
{
	Delegate<_bool> OpenDealDele;
	OpenDealDele.Bind<CItem, &CItem::Open_DealWindow>(this);
	m_pGameInstance->Subscribe_Event(TEXT("Open_Deal"), OpenDealDele);

	Delegate<_bool> CloseDealDele;
	CloseDealDele.Bind<CItem, &CItem::Close_DealWindow>(this);
	m_pGameInstance->Subscribe_Event(TEXT("Close_Deal"), CloseDealDele);
}

void CItem::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	if (CI_PLAYER(eColliderID))
		m_isCollision = true;
}

HRESULT CItem::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CItem::Ready_PartObjects()
{
	CUI3D_Interaction::DESC InteractionDesc{};

	InteractionDesc.pParentLevelID = &m_eLevelID;
	InteractionDesc.iNumPartObjects = CUI3D_Interaction::PART_END;
	InteractionDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	InteractionDesc.pParentIsCollisioned = &m_isCollision;

	if (FAILED(__super::Add_PartObject(PART_INTERACTION, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"), &InteractionDesc)))
		return E_FAIL;

	/* 나중에 꼭 없애라 */
	if (LEVEL::SHOP != m_eLevelID)
		return S_OK;

	CUI2D_Purchase::DESC PurchaseDesc{};

	PurchaseDesc.pParentLevelID = &m_eLevelID;
	PurchaseDesc.iNumPartObjects = CUI2D_Purchase::PART_END;
	PurchaseDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	PurchaseDesc.pParentPrice = &m_iPrice;
	PurchaseDesc.pParentisDeal = &m_isDeal;

	if (FAILED(__super::Add_PartObject(PART_PURCHASE, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI2D_Purchase"), &PurchaseDesc)))
		return E_FAIL;

	return S_OK;
}

void CItem::Free()
{
	__super::Free();
}