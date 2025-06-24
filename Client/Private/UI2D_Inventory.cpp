#include "UI2D_Inventory.h"
#include "GameInstance.h"

#include "UI.h"
#include "UI2D_InventorySlot.h"

#include "Inventory.h"

CUI2D_Inventory::CUI2D_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_Inventory::CUI2D_Inventory(const CUI2D_Inventory& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_Inventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_Inventory::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentIsOnInven = pDesc->pParentIsOnInven;
	m_pInventory = pDesc->pInventory;

	if (nullptr == m_pInventory)
		return E_FAIL;

	Safe_AddRef(m_pInventory);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	SetPos_InvenSlots();

	XMStoreFloat3(&m_vOffInvenPos, XMVectorSet(-700.f, 0.f, 0.f, 1.f));
	XMStoreFloat3(&m_vOnInvenPos, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	m_fDuration = 0.35f;

	return S_OK;
}

void CUI2D_Inventory::Priority_Update(_float fTimeDelta)
{
	if (!(*m_pParentIsOnInven))
		return;

	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_Inventory::Update(_float fTimeDelta)
{
	if (!(*m_pParentIsOnInven) && 0.f < m_fTimeAcc)
	{
		m_fTimeAcc -= fTimeDelta;
		m_fTimeAcc = max(m_fTimeAcc, 0.f);
	}
	else if ((*m_pParentIsOnInven) && m_fTimeAcc <= m_fDuration)
	{
		m_fTimeAcc += fTimeDelta;
		m_fTimeAcc = min(m_fTimeAcc, m_fDuration);
	}

	if (0.f >= m_fTimeAcc)
	{
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vOffInvenPos), 1.f));
	}
	else if (m_fDuration <= m_fTimeAcc)
	{
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vOnInvenPos), 1.f));
	}
	else
	{
		_float fRatio = m_fTimeAcc / m_fDuration;
		XMStoreFloat3(&m_vLerpPos, XMVectorLerp(XMLoadFloat3(&m_vOffInvenPos), XMLoadFloat3(&m_vOnInvenPos), fRatio));
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vLerpPos), 1.f));
	}

	return 	__super::Update(fTimeDelta);
}

void CUI2D_Inventory::Late_Update(_float fTimeDelta)
{
	Update_Slots();
	Update_Seletor();
	Update_Shield();

	__super::Late_Update(fTimeDelta);

	if ((*m_pParentIsOnInven))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI2D_Inventory::Render()
{
	_int iCoin = m_pInventory->Get_Coin();
	const wstring& strCoin = to_wstring(iCoin);

	m_pGameInstance->Draw_Font(TEXT("Font_Money"), strCoin.c_str(), _float2(m_vLerpPos.x + 220.f, m_vLerpPos.y + 132.5f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

void CUI2D_Inventory::Update_Slots()
{
	for (_uint i = CInventory::SLOT_USEITEM0; i < CInventory::SLOT_END; i++)
	{
		CInventory::INVEN_SLOT InvenSlot = m_pInventory->Get_Slot(static_cast<CInventory::SLOT>(i));
		if (InvenSlot.bHasItem)
		{
			m_InvenSlots[i]->Set_UIVisible(CUI2D_InventorySlot::PART_ITEMS, true);
			m_InvenSlots[i]->Set_TextureIndex(CUI2D_InventorySlot::PART_ITEMS, ENUM_CLASS(InvenSlot.eType));
		}
		else
		{
			m_InvenSlots[i]->Set_UIVisible(CUI2D_InventorySlot::PART_ITEMS, false);
		}

		m_InvenSlots[i]->Set_ItemCount(InvenSlot.iCount);
	}
}

void CUI2D_Inventory::Update_Seletor()
{
	for (auto& pSlot : m_InvenSlots)
		pSlot->Set_Selected(false);

	m_InvenSlots[m_pInventory->Get_SeletSlotIndex()]->Set_Selected(true);
}

void CUI2D_Inventory::Update_Shield()
{
	if (m_pInventory->Get_isShield())
	{
		m_InvenSlots[CInventory::SLOT_PASSIVEITEM]->Set_UIVisible(CUI2D_InventorySlot::PART_ITEMS, true);
		m_InvenSlots[CInventory::SLOT_PASSIVEITEM]->Set_TextureIndex(CUI2D_InventorySlot::PART_ITEMS, ENUM_CLASS(ITEM_TYPE::SHILED));
	}

}

void CUI2D_Inventory::SetPos_InvenSlots()
{
	m_InvenSlots[CInventory::SLOT_PASSIVEITEM]->Set_State(STATE::POSITION, XMVectorSet(0.f, 80.f, 0.f, 1.f));

	for (_uint i = 0; i < CInventory::SLOT_USEITEM3; i++)
		m_InvenSlots[CInventory::SLOT_USEITEM0 + i]->Set_State(STATE::POSITION, XMVectorSet((80.f * i), -65.f, 0.f, 1.f));

	for (_uint i = 0; i < 3; i++)
		m_InvenSlots[CInventory::SLOT_WEAPON0 + i]->Set_State(STATE::POSITION, XMVectorSet((80.f * i), -240.f, 0.f, 1.f));

	m_InvenSlots[CInventory::SLOT_USEITEM0]->Set_Selected(true);
}

HRESULT CUI2D_Inventory::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_Inventory::Ready_PartObjects()
{
	CUI::DESC MaskDesc{};

	MaskDesc.pParentLevelID = m_pLevelID;
	MaskDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	MaskDesc.fSizeX = g_iWinSizeX;
	MaskDesc.fSizeY = g_iWinSizeY;
	MaskDesc.fX = g_iWinSizeX * 0.5f;
	MaskDesc.fY = g_iWinSizeY * 0.5f;
	MaskDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIMask");
	MaskDesc.eUIPass = CUI::PASS_BLEND_POINT;

	if (FAILED(__super::Add_PartObject(PART_MASK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &MaskDesc)))
		return E_FAIL;


	for (_uint i = PART_PASSIVEITEMSLOT_START; i <= PART_WEAPONSLOT_END; i++)
	{
		CUI2D_InventorySlot::DESC SlotDesc{};
		SlotDesc.iNumPartObjects = CUI2D_InventorySlot::PART_END;
		SlotDesc.pParentLevelID = m_pLevelID;
		SlotDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		SlotDesc.pParentIsOnInven = m_pParentIsOnInven;
		SlotDesc.pInventory = m_pInventory;

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_InventorySlot"), &SlotDesc)))
			return E_FAIL;
		
		if (nullptr != m_PartObjects[i])
		{
			if (CUI2D_InventorySlot* pInvenSlot = dynamic_cast<CUI2D_InventorySlot*>(m_PartObjects[i]))
				m_InvenSlots.push_back(pInvenSlot);
		}

		++m_iNumInvenSlots;
	}

	for (auto& pInvenSlot : m_InvenSlots)
		Safe_AddRef(pInvenSlot);

	CUI::DESC DividerDesc{};
	DividerDesc.pParentLevelID = m_pLevelID;
	DividerDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	DividerDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIDivider");
	DividerDesc.fSizeX = 511.3f;
	DividerDesc.fSizeY = 8.f;
	DividerDesc.fX = g_iWinSizeX * 0.34f;

	DividerDesc.fY = g_iWinSizeY * 0.33f;
	if (FAILED(__super::Add_PartObject(PART_PASSIVE_DIVIDER, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &DividerDesc)))
		return E_FAIL;

	DividerDesc.fY = (g_iWinSizeY * 0.53f);
	if (FAILED(__super::Add_PartObject(PART_USEITEM_DIVIDER, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &DividerDesc)))
		return E_FAIL;

	DividerDesc.fY = (g_iWinSizeY * 0.77f);
	if (FAILED(__super::Add_PartObject(PART_WEAPON_DIVIDER, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &DividerDesc)))
		return E_FAIL;

	CUI::DESC TextDesc{};

	TextDesc.pParentLevelID = m_pLevelID;
	TextDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	TextDesc.eUIPass = CUI::UI_PASS::PASS_BLEND_POINT;
	TextDesc.fSizeX = 46.8f;
	TextDesc.fSizeY = 30.4f;
	TextDesc.fX = g_iWinSizeX * 0.15f;
	TextDesc.fY = g_iWinSizeY * 0.11f;
	TextDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIMoneyText");

	if (FAILED(__super::Add_PartObject(PART_MONEYTEXT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &TextDesc)))
		return E_FAIL;

	TextDesc.fSizeX = 21.75f;
	TextDesc.fSizeY = 33.75f;
	TextDesc.fX = g_iWinSizeX * 0.15f;
	TextDesc.fY = (g_iWinSizeY * 0.3f);
	TextDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIPassiveItemText");

	if (FAILED(__super::Add_PartObject(PART_PASSIVEITEMTEXT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &TextDesc)))
		return E_FAIL;

	TextDesc.fSizeX = 109.5f;
	TextDesc.fSizeY = 34.5f;
	TextDesc.fX = g_iWinSizeX * 0.18f;
	TextDesc.fY = g_iWinSizeY * 0.5f;
	TextDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIUseItemText");

	if (FAILED(__super::Add_PartObject(PART_USEITEMTEXT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &TextDesc)))
		return E_FAIL;


	TextDesc.fSizeX = 99.5f;
	TextDesc.fSizeY = 38.5f;
	TextDesc.fX = g_iWinSizeX * 0.18f;
	TextDesc.fY = g_iWinSizeY * 0.73f;
	TextDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIWeaponText");

	if (FAILED(__super::Add_PartObject(PART_WEAPONTEXT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &TextDesc)))
		return E_FAIL;

	CUI::DESC TABKeyDesc{};

	TABKeyDesc.pParentLevelID = m_pLevelID;
	TABKeyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	TABKeyDesc.fSizeX = 102.4f;
	TABKeyDesc.fSizeY = 66.4f;
	TABKeyDesc.fX = 100.f;
	TABKeyDesc.fY = g_iWinSizeY * 0.075f;
	TABKeyDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_TABKeyBoard");

	if (FAILED(__super::Add_PartObject(PART_TAB, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &TABKeyDesc)))
		return E_FAIL;

	CUI::DESC LineDesc{};

	LineDesc.pParentLevelID = m_pLevelID;
	LineDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	LineDesc.fSizeX = 244.5f;
	LineDesc.fSizeY = 7.f;
	LineDesc.fX = g_iWinSizeX * 0.23f;
	LineDesc.fY = g_iWinSizeY * 0.14f;
	LineDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UILine");

	if (FAILED(__super::Add_PartObject(PART_LINE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &LineDesc)))
		return E_FAIL;

	CUI::DESC HexagonDesc{};

	HexagonDesc.pParentLevelID = m_pLevelID;
	HexagonDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HexagonDesc.fSizeX = 170.4f;
	HexagonDesc.fSizeY = 195.6f;
	HexagonDesc.fX = (g_iWinSizeX * 0.45f);
	HexagonDesc.fY = g_iWinSizeY * 0.15f;
	HexagonDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIHexagon");

	if (FAILED(__super::Add_PartObject(PART_HEXAGON, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &HexagonDesc)))
		return E_FAIL;

	CUI::DESC CoinIconDesc{};

	CoinIconDesc.pParentLevelID = m_pLevelID;
	CoinIconDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	CoinIconDesc.fSizeX = 33.1f;
	CoinIconDesc.fSizeY = 33.7f;
	CoinIconDesc.fX = (g_iWinSizeX * 0.15f);
	CoinIconDesc.fY = g_iWinSizeY * 0.2f;
	CoinIconDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UICoinIcon");

	if (FAILED(__super::Add_PartObject(PART_COINICON, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &CoinIconDesc)))
		return E_FAIL;

	return S_OK;
}

CUI2D_Inventory* CUI2D_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_Inventory* pInstance = new CUI2D_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_Inventory::Clone(void* pArg)
{
	CUI2D_Inventory* pInstance = new CUI2D_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_Inventory::Free()
{
	__super::Free();

	Safe_Release(m_pInventory);

	for (auto& pInvenSlot : m_InvenSlots)
		Safe_Release(pInvenSlot);
	m_InvenSlots.clear();
}
